#include "../minishell.h"

// char **extract_command_args(t_token *start, t_token *end)
// {
//     int count = 0;
//     t_token *tmp = start;
    
//     // Count word tokens
//     while (tmp && tmp != end) {
//         if (tmp->type == TOKEN_WORD)
//             count++;
//         tmp = tmp->next;
//     }
    
//     // Allocate array for arguments
//     char **args = malloc(sizeof(char *) * (count + 1));
//     if (!args)
//         return NULL;
    
//     // Copy argument values
//     int i = 0;
//     tmp = start;
//     while (tmp && tmp != end) {
//         if (tmp->type == TOKEN_WORD) {
//             args[i] = ft_strdup(tmp->value);
//             if (!args[i]) {
//                 // Clean up on error
//                 while (--i >= 0)
//                     free(args[i]);
//                 free(args);
//                 return NULL;
//             }
//             i++;
//         }
//         tmp = tmp->next;
//     }
//     args[i] = NULL; // Null-terminate the array
    
//     return args;
// }

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
    t_token *tmp = tokens;
    int count = 0;
    
    // Count the number of pipes
    while (tmp && tmp->type != TOKEN_SEMIC) {
        if (tmp->type == TOKEN_PIPE)
            count++;
        tmp = tmp->next;
    }
    count++; // Number of commands = number of pipes + 1
    
    // Allocate memory for the pipe commands
    t_pipe_command *pipes = malloc(sizeof(t_pipe_command) * count);
    if (!pipes)
        return NULL;
    
    // Initialize the pipe commands
    int i = 0;
    while (i < count) {
        pipes[i].count_pipe = count;
        pipes[i].index = i;
        pipes[i].pipe_id = -1;
        pipes[i].pipe_stdin = STDIN_FILENO;
        pipes[i].pipe_stdout = STDOUT_FILENO;
        pipes[i].args = NULL;
        i++;
    }
    
    // Parse the tokens into pipe commands
    tmp = tokens;
    t_token *cmd_start = tmp;
    int cmd_index = 0;
    
    while (tmp && tmp->type != TOKEN_SEMIC) {
        if (tmp->type == TOKEN_PIPE) {
            // Collect all tokens between cmd_start and tmp as args for the current command
            int arg_count = 0;
            t_token *curr = cmd_start;
            
            // Count arguments
            while (curr != tmp) {
                if (curr->type == TOKEN_WORD)
                    arg_count++;
                curr = curr->next;
            }
            
            // Allocate memory for arguments
            pipes[cmd_index].args = malloc(sizeof(char*) * (arg_count + 1));// You can remove +1 in here!!
            if (!pipes[cmd_index].args)
            {
                free_pipe_command(pipes, count);
                return NULL;
            }
            
            // Copy arguments
            curr = cmd_start;
            int arg_index = 0;
            while (curr != tmp) {
                if(curr->type == TOKEN_REDIRECT_OUT || curr->type == TOKEN_REDIRECT_APPEND || 
                    curr->type == TOKEN_REDIRECT_IN || curr->type == TOKEN_HEREDOC)
                {
                    // t_token *redirect_token = curr;
                    curr = curr->next;
                    if(curr->type == TOKEN_WORD)
                    {
                        curr = curr->next;
                        continue;
                    }
                }
                else if (curr->type == TOKEN_WORD) {
                    pipes[cmd_index].args[arg_index] = ft_strdup(curr->value);
                    arg_index++;
                    curr = curr->next;
                }
                else
                    curr = curr->next;
                // curr = curr->next; // Be carefull to say next
            }
            pipes[cmd_index].args[arg_index] = NULL;
            
            // Debug output
            DEBUG_PRINT(BLUE "Command %d args: ", cmd_index);
            for (int i = 0; pipes[cmd_index].args[i]; i++) {
                DEBUG_PRINT(BLUE " [DEBUG] '%s' ", pipes[cmd_index].args[i]);
            }
            DEBUG_PRINT(BLUE "\n");
            
            // Move to the next command
            cmd_start = tmp->next;
            cmd_index++;
        }
        tmp = tmp->next;
    }
    
    // Handle the last command
    if (cmd_start) {
        int arg_count = 0;
        t_token *arg_tmp = cmd_start;
        
        // Count arguments
        while (arg_tmp && arg_tmp->type != TOKEN_SEMIC) {
            if (arg_tmp->type == TOKEN_WORD)
                arg_count++;
            arg_tmp = arg_tmp->next;
        }
        
        // Allocate memory for arguments
        pipes[cmd_index].args = malloc(sizeof(char*) * (arg_count + 1));// You can remove it "+1"
        if (!pipes[cmd_index].args)
        {
            free_pipe_command(pipes, count);
            return NULL;
        }
        
        // Copy arguments
        arg_tmp = cmd_start;
        int arg_index = 0;
        while (arg_tmp && arg_tmp->type != TOKEN_SEMIC) {
            if(arg_tmp->type == TOKEN_REDIRECT_OUT || arg_tmp->type == TOKEN_REDIRECT_APPEND || 
                arg_tmp->type == TOKEN_REDIRECT_IN || arg_tmp->type == TOKEN_HEREDOC)
            {
                // t_token *redirect_token = arg_tmp;
                arg_tmp = arg_tmp->next;
                if(arg_tmp->type == TOKEN_WORD)
                {
                    arg_tmp = arg_tmp->next;
                    continue;
                }
            }
            else if (arg_tmp->type == TOKEN_WORD) {
                pipes[cmd_index].args[arg_index] = ft_strdup(arg_tmp->value);
                arg_index++;
                arg_tmp = arg_tmp->next;
            }
            else
                arg_tmp = arg_tmp->next;
            // curr = curr->next; // Be carefull to say next
        }
        pipes[cmd_index].args[arg_index] = NULL;
        // while (arg_tmp && arg_tmp->type != TOKEN_SEMIC) {
        //     if (arg_tmp->type == TOKEN_WORD) {
        //         pipes[cmd_index].args[arg_index] = ft_strdup(arg_tmp->value);
        //         arg_index++;
        //     }
        //     arg_tmp = arg_tmp->next;
        // }
        // pipes[cmd_index].args[arg_index] = NULL;
        
        // Debug output
        DEBUG_PRINT(BLUE "Command %d args: ", cmd_index);
        for (int i = 0; pipes[cmd_index].args[i]; i++) {
            DEBUG_PRINT(BLUE " [DEBUG] '%s' ", pipes[cmd_index].args[i]);
        }
        DEBUG_PRINT(BLUE "\n");
    }
    
    *pipe_count = count;
    DEBUG_PRINT(BLUE "Detected %d commands in pipe chain\n", count);
    
    return pipes;
}


void    handle_pipe_redirection(t_token *cmd_start, t_token *cmd_end, t_env *env)
{
    t_token *curr = cmd_start;
    int fd = STDIN_FILENO;

    while(curr && curr != cmd_end)
    {
        if(curr->type == TOKEN_REDIRECT_OUT)
        {
            if(curr->next && curr->next->type == TOKEN_WORD)
            {
                fd = open(curr->next->value, O_WRONLY | O_CREAT | O_TRUNC, 0644);
                if(fd < 0)
                {
                    perror("open failed");
                    return;
                }
                if(dup2(fd, STDOUT_FILENO) == -1)
                {
                    perror("dup2 failed");
                    close(fd);
                    return;
                }
                close(fd);
                DEBUG_PRINT(RED"Redirecting output to '%s'\n" RESET,curr->next->value);
            }
        }
        else if(curr->type == TOKEN_REDIRECT_APPEND)
        {
            if(curr->next && curr->next->type == TOKEN_WORD)
            {
                fd = open(curr->next->value, O_WRONLY | O_CREAT | O_APPEND, 0644);
                if(fd < 0)
                {
                    perror("open failed");
                    return;
                }
                if(dup2(fd, STDOUT_FILENO) == -1)
                {
                    perror("dup2 failed");
                    close(fd);
                    return;
                }
                close(fd);
                DEBUG_PRINT(RED"Redirecting output to '%s'\n" RESET,curr->next->value);
            }
        }
        else if(curr->type == TOKEN_REDIRECT_IN)
        {
            if(curr->next && curr->next->type == TOKEN_WORD)
            {
                fd = open(curr->next->value, O_RDONLY);
                if(fd < 0)
                {
                    perror("open failed");
                    return;
                }
                if(dup2(fd, STDOUT_FILENO) == -1)
                {
                    perror("dup2 failed");
                    close(fd);
                    return;
                }
                close(fd);
                DEBUG_PRINT(RED"Redirecting output to '%s'\n" RESET,curr->next->value);
            }
        }
        else if(curr->type == TOKEN_HEREDOC)
        {
            if(curr->next && curr->next->type == TOKEN_WORD)
            {
                char *heredoc_input = handler_heredoc(curr->next->value, env);
                if (heredoc_input) {
                int pipe_fd[2];
                if (pipe(pipe_fd) == -1) {
                    perror("pipe failed");
                    free(heredoc_input);
                    return;
                }
                write(pipe_fd[1], heredoc_input, ft_strlen(heredoc_input));
                close(pipe_fd[1]);
                if (dup2(pipe_fd[0], STDIN_FILENO) == -1) {
                    perror("dup2 failed");
                    close(pipe_fd[0]);
                    free(heredoc_input);
                    return;
                }
                close(pipe_fd[0]);
                free(heredoc_input);
                DEBUG_PRINT(BLUE "Applied heredoc with delimiter '%s'\n" RESET, curr->next->value);
                }
            }
        }
        curr = curr->next;
    }
}


// static int **setup_pipes(t_pipe_command *pipes, int pipe_count)
// {
//     if (pipe_count <= 1) {
//         DEBUG_PRINT(RED "No need for pipes with only one command\n" RESET);
//         return NULL;
//     }
    
//     // Allocate pipe array
//     int **pipe_fds = malloc(sizeof(int *) * (pipe_count - 1));
//     if (!pipe_fds) {
//         DEBUG_PRINT(RED "Failed to allocate pipe array\n" RESET);
//         return NULL;
//     }
    
//     // Create each pipe
//     for (int i = 0; i < pipe_count - 1; i++) {
//         pipe_fds[i] = malloc(sizeof(int) * 2);
//         if (!pipe_fds[i]) {
//             DEBUG_PRINT(RED "Failed to allocate pipe %d\n" RESET, i);
            
//             // Clean up already allocated pipes
//             for (int j = 0; j < i; j++) {
//                 free(pipe_fds[j]);
//             }
//             free(pipe_fds);
//             return NULL;
//         }
        
//         if (pipe(pipe_fds[i]) == -1) {
//             DEBUG_PRINT(RED "Failed to create pipe %d\n" RESET, i);
//             perror("pipe");
            
//             // Clean up already allocated pipes
//             for (int j = 0; j <= i; j++) {
//                 free(pipe_fds[j]);
//             }
//             free(pipe_fds);
//             return NULL;
//         }
        
//         // Connect pipes to commands
//         pipes[i].pipe_stdout = pipe_fds[i][1];     // Write end of current pipe
//         pipes[i + 1].pipe_stdin = pipe_fds[i][0];  // Read end of current pipe
        
//         DEBUG_PRINT(BLUE "Created pipe %d: read_fd=%d, write_fd=%d\n" RESET, 
//                    i, pipe_fds[i][0], pipe_fds[i][1]);
//     }
    
//     return pipe_fds;
// }

// // static void child_process_pipe(t_pipe_command *pipe_command, int **pipe_fds, t_env *env)
// // {
// //     // Stdin from the appropriate pipe if not the first command!!!
// //     if(pipe_command->pipe_stdin != STDIN_FILENO)
// //     {
// //         if(dup2(pipe_command->pipe_stdin, STDIN_FILENO) == -1)
// //         {
// //             perror("dup2 failed on stdin");
// //             exit(EXIT_FAILURE);
// //         }
// //     }

// //     if(pipe_command->pipe_stdout != STDOUT_FILENO)
// //     {
// //         if(dup2(pipe_command->pipe_stdout, STDOUT_FILENO) == -1)
// //         {
// //             perror("dup2 failed on stdin");
// //             exit(EXIT_FAILURE);
// //         }
// //     }

// //     int i = 0;
// //     while(i < pipe_command->count_pipe -1)
// //     {
// //         close(pipe_fds[i][0]); // read end
// //         close(pipe_fds[i][1]); // write end
// //         i++;
// //     }

// //     if(builtin_check(pipe_command->args))
// //     {
// //         run_builtin(pipe_command->args, env);
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
// void execute_pipes(t_pipe_command *pipes, t_env *env)
// {
//     int pipe_count = pipes[0].count_pipe;
//     DEBUG_PRINT(BLUE "Executing pipe chain with %d commands\n" RESET, pipe_count);
    
//     // Set up pipes
//     int **pipe_fds = NULL;
//     if (pipe_count > 1) {
//         pipe_fds = setup_pipes(pipes, pipe_count);
//         if (!pipe_fds && pipe_count > 1) {
//             DEBUG_PRINT(RED "Failed to set up pipes\n" RESET);
//             free_pipe_command(pipes, pipe_count);
//             return;
//         }
//     }
    
//     // Fork processes for each command
//     for (int i = 0; i < pipe_count; i++) {
//         pipes[i].pipe_id = fork();
        
//         if (pipes[i].pipe_id == -1) {
//             perror("fork failed");
//             continue;
//         }
        
//         if (pipes[i].pipe_id == 0) {
//             // Child process
//             DEBUG_PRINT(BLUE "Child process %d (command: %s)\n" RESET, 
//                        i, pipes[i].args[0]);
            
//             // Redirect stdin/stdout for this command
//             if (pipes[i].pipe_stdin != STDIN_FILENO) {
//                 DEBUG_PRINT(BLUE "Redirecting stdin from fd %d\n" RESET, pipes[i].pipe_stdin);
//                 if (dup2(pipes[i].pipe_stdin, STDIN_FILENO) == -1) {
//                     perror("dup2 failed on stdin");
//                     exit(EXIT_FAILURE);
//                 }
//             }
            
//             if (pipes[i].pipe_stdout != STDOUT_FILENO) {
//                 DEBUG_PRINT(BLUE "Redirecting stdout to fd %d\n" RESET, pipes[i].pipe_stdout);
//                 if (dup2(pipes[i].pipe_stdout, STDOUT_FILENO) == -1) {
//                     perror("dup2 failed on stdout");
//                     exit(EXIT_FAILURE);
//                 }
//             }
            
//             // Close all pipe file descriptors
//             if (pipe_count > 1 && pipe_fds) {
//                 for (int j = 0; j < pipe_count - 1; j++) {
//                     close(pipe_fds[j][0]);
//                     close(pipe_fds[j][1]);
//                 }
//             }
            
//             // Execute the command
//             if (builtin_check(pipes[i].args)) {
//                 DEBUG_PRINT(BLUE "Executing builtin command: %s\n" RESET, pipes[i].args[0]);
//                 run_builtin(pipes[i].args, env);
//                 exit(env->exit_code);
//             } else {
//                 DEBUG_PRINT(BLUE "Executing external command: %s\n" RESET, pipes[i].args[0]);
//                 exec_command(pipes[i].args, env, STDOUT_FILENO);
                
//                 // If we get here, there was an error
//                 if (env->exit_code != 0) {
//                     exit(env->exit_code);
//                 } else {
//                     exit(EXIT_FAILURE);
//                 }
//             }
//         }
//     }
    
//     // Parent process: close all pipe fds
//     if (pipe_count > 1 && pipe_fds) {
//         for (int i = 0; i < pipe_count - 1; i++) {
//             close(pipe_fds[i][0]);
//             close(pipe_fds[i][1]);
//             free(pipe_fds[i]);
//         }
//         free(pipe_fds);
//     }
    
//     // Wait for all child processes
//     for (int i = 0; i < pipe_count; i++) {
//         if (pipes[i].pipe_id > 0) {
//             int status;
//             DEBUG_PRINT(BLUE "Waiting for child process %d (pid: %d)\n" RESET, 
//                        i, pipes[i].pipe_id);
//             waitpid(pipes[i].pipe_id, &status, 0);
            
//             if (WIFEXITED(status)) {
//                 env->exit_code = WEXITSTATUS(status);
//                 DEBUG_PRINT(BLUE "Child process %d exited with status %d\n" RESET, 
//                            i, env->exit_code);
//             } else if (WIFSIGNALED(status)) {
//                 env->exit_code = 128 + WTERMSIG(status);
//                 DEBUG_PRINT(BLUE "Child process %d terminated by signal %d\n" RESET, 
//                            i, WTERMSIG(status));
//             }
//         }
//     }
    
//     // Free the pipe command structure
//     free_pipe_command(pipes, pipe_count);
//     DEBUG_PRINT(BLUE "Pipe execution completed\n" RESET);
// }

void execute_pipes(t_pipe_command *pipes, t_env *env, t_token *tokens)
{
    int i;
    int pipes_count = pipes[0].count_pipe;
    int pipe_fds[pipes_count - 1][2]; // Array to hold pipe file descriptors
    pid_t pids[pipes_count]; // Array to hold process IDs


    // t_token *cmd_start = NULL;
    // t_token *cmd_end = NULL;
    // t_token *tokens = NULL;
    
    DEBUG_PRINT(BLUE "Executing pipe chain with %d commands\n" RESET, pipes_count);
    
    // Create pipes
    for (i = 0; i < pipes_count - 1; i++) {
        if (pipe(pipe_fds[i]) == -1) {
            perror("pipe failed");
            return;
        }
        DEBUG_PRINT(BLUE "Created pipe %d: read_fd=%d, write_fd=%d\n" RESET, 
                   i, pipe_fds[i][0], pipe_fds[i][1]);
    }
    
    t_token **cmd_start = malloc(sizeof(t_token *) * pipes_count);
    t_token **cmd_ends = malloc(sizeof(t_token *) * pipes_count);
    if(!cmd_start || !cmd_ends)
    {
        free(cmd_start);
        free(cmd_ends);
        return;
    }

    // Identify token boundaries for each command segment
    t_token *curr = tokens; // You'll need to pass the original tokens list into this function
    t_token *start = curr;
    int cmd_idx = 0;
    
    while (curr) {
        if (curr->type == TOKEN_PIPE || curr->type == TOKEN_SEMIC) {
            cmd_ends[cmd_idx] = curr;
            cmd_idx++;
            if (curr->next) {
                start = curr->next;
                cmd_start[cmd_idx] = start;
            }
        }
        curr = curr->next;
    }

    // Create processes for each command
    for (i = 0; i < pipes_count; i++) {
        pids[i] = fork();
        
        if (pids[i] < 0) {
            perror("fork failed");
            return;
        }
        
        if (pids[i] == 0) { // Child process
            DEBUG_PRINT(BLUE "Child process %d (command: %s)\n" RESET, i, pipes[i].args[0]);
            
            // Set up stdin for all but the first command
            if (i > 0) {
                DEBUG_PRINT(BLUE "Redirecting stdin from fd %d\n" RESET, pipe_fds[i-1][0]);
                if (dup2(pipe_fds[i-1][0], STDIN_FILENO) == -1) {
                    perror("dup2 for stdin failed");
                    exit(EXIT_FAILURE);
                }
            }
            
            // Set up stdout for all but the last command
            if (i < pipes_count - 1) {
                DEBUG_PRINT(BLUE "Redirecting stdout to fd %d\n" RESET, pipe_fds[i][1]);
                if (dup2(pipe_fds[i][1], STDOUT_FILENO) == -1) {
                    perror("dup2 for stdout failed");
                    exit(EXIT_FAILURE);
                }
            }
            handle_pipe_redirection(cmd_start[i], cmd_ends[i], env);
            // Close all pipe file descriptors
            for (int j = 0; j < pipes_count - 1; j++) {
                close(pipe_fds[j][0]);
                close(pipe_fds[j][1]);
            }
            
            // Execute the command
            if (builtin_check(pipes[i].args)) {
                DEBUG_PRINT(BLUE "Executing builtin command: %s\n" RESET, pipes[i].args[0]);
                run_builtin(pipes[i].args, env);
                exit(env->exit_code);
            } else {
                DEBUG_PRINT(BLUE "Executing external command: %s\n" RESET, pipes[i].args[0]);
                char *path = find_path(env);
                char *exec_path = NULL;
                
                if (ft_strchr(pipes[i].args[0], '/')) {
                    // Path included in command name
                    if (execve(pipes[i].args[0], pipes[i].args, env->envp) == -1) {
                        perror("execve failed");
                        exit(EXIT_FAILURE);
                    }
                } else {
                    // Look for command in PATH
                    exec_path = find_exec(pipes[i].args[0], path, 0, 5);
                    if (!exec_path) {
                        printf("minishell: %s: command not found.\n", pipes[i].args[0]);
                        exit(127);
                    }
                    if (execve(exec_path, pipes[i].args, env->envp) == -1) {
                        perror("execve failed");
                        free(exec_path);
                        exit(EXIT_FAILURE);
                    }
                    free(exec_path);
                }
            }
            
            // Should never reach here
            exit(EXIT_FAILURE);
        }
    }
    
    // Parent process
    // Close all pipe file descriptors in the parent
    for (i = 0; i < pipes_count - 1; i++) {
        close(pipe_fds[i][0]);
        close(pipe_fds[i][1]);
    }
    
    // Wait for all child processes to finish
    for (i = 0; i < pipes_count; i++) {
        int status;
        DEBUG_PRINT(BLUE "Waiting for child process %d (pid: %d)\n" RESET, i, pids[i]);
        waitpid(pids[i], &status, 0);
        
        if (WIFEXITED(status)) {
            DEBUG_PRINT(BLUE "Child process %d exited with status %d\n" RESET, 
                       i, WEXITSTATUS(status));
            env->exit_code = WEXITSTATUS(status);
        } else if (WIFSIGNALED(status)) {
            DEBUG_PRINT(BLUE "Child process %d terminated by signal %d\n" RESET, 
                       i, WTERMSIG(status));
            env->exit_code = 128 + WTERMSIG(status);
        }
    }
    free(cmd_start);
    free(cmd_ends);
    DEBUG_PRINT(BLUE "Pipe execution completed\n" RESET);
}