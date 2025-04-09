#include "../minishell.h"

char **extract_command_args(t_token *start, t_token *end)
{
    int count = 0;
    t_token *tmp = start;
    
    // Count word tokens
    while (tmp && tmp != end) {
        if (tmp->type == TOKEN_WORD)
            count++;
        tmp = tmp->next;
    }
    
    // Allocate array for arguments
    char **args = malloc(sizeof(char *) * (count + 1));
    if (!args)
        return NULL;
    
    // Copy argument values
    int i = 0;
    tmp = start;
    while (tmp && tmp != end) {
        if (tmp->type == TOKEN_WORD) {
            args[i] = ft_strdup(tmp->value);
            if (!args[i]) {
                // Clean up on error
                while (--i >= 0)
                    free(args[i]);
                free(args);
                return NULL;
            }
            i++;
        }
        tmp = tmp->next;
    }
    args[i] = NULL; // Null-terminate the array
    
    return args;
}

void    free_pipe_command(t_pipe_command *pipes, int pipe_count)
{
    if(!pipes)
        return;
    int i = 0;
    while (i < pipe_count)
    {
        if(pipes[i].args)
        {
            clean_2d(pipes[i].args);
            pipes[i].args = NULL;
        }
        i++;
    }
    free(pipes);
}

// static int count_pipes(t_token *tokens)
// {
//     int count = 0;
//     t_token *tmp = tokens;
//     while (tmp)
//     {
//         if (tmp->type == TOKEN_PIPE)
//             count++;
//         tmp = tmp->next;
//     }
//     return count;
// }

t_pipe_command *parse_pipe(t_token *tokens, int *pipe_count)
{
    // Count pipes and calculate command count
    *pipe_count = 1; // Start with 1 command
    t_token *count_tmp = tokens;
    while (count_tmp) {
        if (count_tmp->type == TOKEN_PIPE)
            (*pipe_count)++;
        count_tmp = count_tmp->next;
    }
    
    DEBUG_PRINT(BLUE "Detected %d commands in pipe chain\n" RESET, *pipe_count);
    
    // Allocate pipe command array
    t_pipe_command *pipes = malloc(sizeof(t_pipe_command) * (*pipe_count));
    if (!pipes) {
        DEBUG_PRINT(RED "Failed to allocate pipe commands\n" RESET);
        return NULL;
    }
    
    // Initialize all fields
    for (int i = 0; i < *pipe_count; i++) {
        pipes[i].count_pipe = *pipe_count;
        pipes[i].index = i;
        pipes[i].pipe_id = -1;
        pipes[i].pipe_stdin = (i == 0) ? STDIN_FILENO : -1;
        pipes[i].pipe_stdout = (i == *pipe_count - 1) ? STDOUT_FILENO : -1;
        pipes[i].args = NULL;
    }
    
    // Parse each command
    t_token *tmp = tokens;
    t_token *cmd_start = tokens;
    int cmd_index = 0;
    
    while (tmp && cmd_index < *pipe_count) {
        // Find the end of this command (next pipe or end of tokens)
        t_token *cmd_end = tmp;
        while (cmd_end && cmd_end->type != TOKEN_PIPE)
            cmd_end = cmd_end->next;
        
        // Extract arguments for this command
        pipes[cmd_index].args = extract_command_args(cmd_start, cmd_end);
        if (!pipes[cmd_index].args) {
            DEBUG_PRINT(RED "Failed to extract command arguments for cmd %d\n" RESET, cmd_index);
            
            // Clean up already allocated commands
            for (int j = 0; j < cmd_index; j++) {
                if (pipes[j].args)
                    clean_2d(pipes[j].args);
            }
            free(pipes);
            return NULL;
        }
        
        // Debug output for this command
        DEBUG_PRINT(BLUE "Command %d args: ", cmd_index);
        for (int i = 0; pipes[cmd_index].args[i]; i++) {
            DEBUG_PRINT("'%s' ", pipes[cmd_index].args[i]);
        }
        DEBUG_PRINT("\n" RESET);
        
        // Move to next command
        cmd_index++;
        if (cmd_end && cmd_end->type == TOKEN_PIPE) {
            cmd_start = cmd_end->next;
            tmp = cmd_end->next;
        } else {
            break; // End of tokens
        }
    }
    
    return pipes;
}


static int **setup_pipes(t_pipe_command *pipes, int pipe_count)
{
    if (pipe_count <= 1) {
        DEBUG_PRINT(RED "No need for pipes with only one command\n" RESET);
        return NULL;
    }
    
    // Allocate pipe array
    int **pipe_fds = malloc(sizeof(int *) * (pipe_count - 1));
    if (!pipe_fds) {
        DEBUG_PRINT(RED "Failed to allocate pipe array\n" RESET);
        return NULL;
    }
    
    // Create each pipe
    for (int i = 0; i < pipe_count - 1; i++) {
        pipe_fds[i] = malloc(sizeof(int) * 2);
        if (!pipe_fds[i]) {
            DEBUG_PRINT(RED "Failed to allocate pipe %d\n" RESET, i);
            
            // Clean up already allocated pipes
            for (int j = 0; j < i; j++) {
                free(pipe_fds[j]);
            }
            free(pipe_fds);
            return NULL;
        }
        
        if (pipe(pipe_fds[i]) == -1) {
            DEBUG_PRINT(RED "Failed to create pipe %d\n" RESET, i);
            perror("pipe");
            
            // Clean up already allocated pipes
            for (int j = 0; j <= i; j++) {
                free(pipe_fds[j]);
            }
            free(pipe_fds);
            return NULL;
        }
        
        // Connect pipes to commands
        pipes[i].pipe_stdout = pipe_fds[i][1];     // Write end of current pipe
        pipes[i + 1].pipe_stdin = pipe_fds[i][0];  // Read end of current pipe
        
        DEBUG_PRINT(BLUE "Created pipe %d: read_fd=%d, write_fd=%d\n" RESET, 
                   i, pipe_fds[i][0], pipe_fds[i][1]);
    }
    
    return pipe_fds;
}

// static void child_process_pipe(t_pipe_command *pipe_command, int **pipe_fds, t_env *env)
// {
//     // Stdin from the appropriate pipe if not the first command!!!
//     if(pipe_command->pipe_stdin != STDIN_FILENO)
//     {
//         if(dup2(pipe_command->pipe_stdin, STDIN_FILENO) == -1)
//         {
//             perror("dup2 failed on stdin");
//             exit(EXIT_FAILURE);
//         }
//     }

//     if(pipe_command->pipe_stdout != STDOUT_FILENO)
//     {
//         if(dup2(pipe_command->pipe_stdout, STDOUT_FILENO) == -1)
//         {
//             perror("dup2 failed on stdin");
//             exit(EXIT_FAILURE);
//         }
//     }

//     int i = 0;
//     while(i < pipe_command->count_pipe -1)
//     {
//         close(pipe_fds[i][0]); // read end
//         close(pipe_fds[i][1]); // write end
//         i++;
//     }

//     if(builtin_check(pipe_command->args))
//     {
//         run_builtin(pipe_command->args, env);
//         exit(env->exit_code);
//     } else
//     {
//         exec_command(pipe_command->args, env, STDOUT_FILENO);
//         if(env->exit_code != 0)
//             exit(env->exit_code);
//         else
//             exit(EXIT_FAILURE);
//     }
// }

// If the function who call wait_for_child is not being static. We can use it when we clean it the command !

// static void main_process(t_pipe_command *pipes, int **pipe_fds, int pipe_count, t_env *env)
// {
//     int status;

//     if(pipe_count == 1)
//     {
//         clean_2d(pipes[0].args);
//         free(pipes);
//         return;
//     }
//     int i =0;
//     while(i < pipe_count)
//     {
//         close(pipe_fds[i][0]);
//         close(pipe_fds[i][1]);
//         free(pipe_fds[i]);
//         i++;
//     }
//     free(pipe_fds);
//     i = 0;
//     while(i < pipe_count)
//     {
//         waitpid(pipes[i].pipe_id, &status, 0);
//         if (WIFEXITED(status))
//             env->exit_code = WEXITSTATUS(status);
//         else if (WIFSIGNALED(status))
//             env->exit_code = 128 + WTERMSIG(status);
//         i++;
//     }
//     free_pipe_command(pipes, pipe_count);
// }
void execute_pipes(t_pipe_command *pipes, t_env *env)
{
    int pipe_count = pipes[0].count_pipe;
    DEBUG_PRINT(BLUE "Executing pipe chain with %d commands\n" RESET, pipe_count);
    
    // Set up pipes
    int **pipe_fds = NULL;
    if (pipe_count > 1) {
        pipe_fds = setup_pipes(pipes, pipe_count);
        if (!pipe_fds && pipe_count > 1) {
            DEBUG_PRINT(RED "Failed to set up pipes\n" RESET);
            free_pipe_command(pipes, pipe_count);
            return;
        }
    }
    
    // Fork processes for each command
    for (int i = 0; i < pipe_count; i++) {
        pipes[i].pipe_id = fork();
        
        if (pipes[i].pipe_id == -1) {
            perror("fork failed");
            continue;
        }
        
        if (pipes[i].pipe_id == 0) {
            // Child process
            DEBUG_PRINT(BLUE "Child process %d (command: %s)\n" RESET, 
                       i, pipes[i].args[0]);
            
            // Redirect stdin/stdout for this command
            if (pipes[i].pipe_stdin != STDIN_FILENO) {
                DEBUG_PRINT(BLUE "Redirecting stdin from fd %d\n" RESET, pipes[i].pipe_stdin);
                if (dup2(pipes[i].pipe_stdin, STDIN_FILENO) == -1) {
                    perror("dup2 failed on stdin");
                    exit(EXIT_FAILURE);
                }
            }
            
            if (pipes[i].pipe_stdout != STDOUT_FILENO) {
                DEBUG_PRINT(BLUE "Redirecting stdout to fd %d\n" RESET, pipes[i].pipe_stdout);
                if (dup2(pipes[i].pipe_stdout, STDOUT_FILENO) == -1) {
                    perror("dup2 failed on stdout");
                    exit(EXIT_FAILURE);
                }
            }
            
            // Close all pipe file descriptors
            if (pipe_count > 1 && pipe_fds) {
                for (int j = 0; j < pipe_count - 1; j++) {
                    close(pipe_fds[j][0]);
                    close(pipe_fds[j][1]);
                }
            }
            
            // Execute the command
            if (builtin_check(pipes[i].args)) {
                DEBUG_PRINT(BLUE "Executing builtin command: %s\n" RESET, pipes[i].args[0]);
                run_builtin(pipes[i].args, env);
                exit(env->exit_code);
            } else {
                DEBUG_PRINT(BLUE "Executing external command: %s\n" RESET, pipes[i].args[0]);
                exec_command(pipes[i].args, env, STDOUT_FILENO);
                
                // If we get here, there was an error
                if (env->exit_code != 0) {
                    exit(env->exit_code);
                } else {
                    exit(EXIT_FAILURE);
                }
            }
        }
    }
    
    // Parent process: close all pipe fds
    if (pipe_count > 1 && pipe_fds) {
        for (int i = 0; i < pipe_count - 1; i++) {
            close(pipe_fds[i][0]);
            close(pipe_fds[i][1]);
            free(pipe_fds[i]);
        }
        free(pipe_fds);
    }
    
    // Wait for all child processes
    for (int i = 0; i < pipe_count; i++) {
        if (pipes[i].pipe_id > 0) {
            int status;
            DEBUG_PRINT(BLUE "Waiting for child process %d (pid: %d)\n" RESET, 
                       i, pipes[i].pipe_id);
            waitpid(pipes[i].pipe_id, &status, 0);
            
            if (WIFEXITED(status)) {
                env->exit_code = WEXITSTATUS(status);
                DEBUG_PRINT(BLUE "Child process %d exited with status %d\n" RESET, 
                           i, env->exit_code);
            } else if (WIFSIGNALED(status)) {
                env->exit_code = 128 + WTERMSIG(status);
                DEBUG_PRINT(BLUE "Child process %d terminated by signal %d\n" RESET, 
                           i, WTERMSIG(status));
            }
        }
    }
    
    // Free the pipe command structure
    free_pipe_command(pipes, pipe_count);
    DEBUG_PRINT(BLUE "Pipe execution completed\n" RESET);
}