#include "../minishell.h"

char **ext_cmd_arg(t_token *start, t_token *end)
{
    if(!start)
        return NULL;

    //Count word tokens
    int count = 0;
    t_token *tmp = start;

    while(tmp && tmp != end)
    {
        if(tmp->type == TOKEN_WORD)
            count++;
        tmp = tmp->next;
    }

    char **args = malloc(sizeof(char*)*(count +1));
    if(!args)
        return NULL;
    
    int i = 0;
    tmp = start;
    while(tmp && tmp != end)
    {
        if(tmp->type == TOKEN_WORD)
        {
            args[i++] = ft_strdup(tmp->value);
            if(!args[i])
            {
                int j = 0;
                while(j < i)
                {
                    free(args[j]);
                    j++;
                }
                free(args);
                return NULL;
            }
            i++;
        }
        tmp = tmp->next;
    }
    args[i] = NULL;

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

static int count_pipes(t_token *tokens)
{
    int count = 0;
    t_token *tmp = tokens;
    while (tmp)
    {
        if (tmp->type == TOKEN_PIPE)
            count++;
        tmp = tmp->next;
    }
    return count;
}
t_pipe_command *parse_pipe(t_token *tokens, int *pipe_count)
{
    t_token *tmp = tokens;
    *pipe_count = count_pipes(tokens) + 1;
    t_pipe_command *pipes = malloc(sizeof(t_pipe_command) * (*pipe_count));
    if (!pipes)
        return NULL;

    /ÖFFF ÖFFFF ENOUGHHH!!!
    // int i = 0;
    // t_token *start = tokens;
    // while (tmp && i < *pipe_count)
    // {
    //     if (i == 0) // Child process open the filee!!
    //         pipes[i].pipe_stdin = STDIN_FILENO;
    //     else
    //         pipes[i].pipe_stdin = -1;
        
    //     if(i == *pipe_count -1)
    //         pipes[i].pipe_stdout = STDOUT_FILENO;
    //     else
    //         pipes[i].pipe_stdout = -1;
        
    //     t_token *end = start;
    //     while(end && end->type != TOKEN_PIPE)
    //         end = end->next;
        
    //     pipes[i].args = ext_cmd_arg(start, end);
    //     if(!pipes[i].args)
    //     {
    //         free_pipe_command(pipes, *pipe_count);
    //         return NULL;
    //     }

    //     if (end && end->type == TOKEN_PIPE)
    //         start = end->next;
    //     else
    //         start = NULL;
        
    //     while(tmp && tmp->type != TOKEN_PIPE)
    //         tmp = tmp->next;
    //     if(tmp)
    //         tmp = tmp->next;
    //     i++;
           
    // }
    return pipes;
}

static int **setup_pipes(t_pipe_command *pipes, int pipe_count)
{
    int **pipe_fds;

    if (pipe_count <= 1)
        return NULL;
    pipe_fds = malloc(sizeof(int *) * (pipe_count - 1));
    if (!pipe_fds)
        return NULL;
    int i = 0;
    while (i < pipe_count - 1)
    {
        pipe_fds[i] = malloc(sizeof(int) * 2);
        if (!pipe_fds[i] || pipe(pipe_fds[i]) == -1)
        {
            int j = 0;
            while(j <= i)
            {
                free(pipe_fds[i]);
                j++;
            }
            free(pipe_fds);
            return NULL;
        }
        pipes[i].pipe_stdout = pipe_fds[i][1];
        pipes[i + 1].pipe_stdin = pipe_fds[i][0];
        i++;
    }
    return pipe_fds;
}

static void child_process_pipe(t_pipe_command *pipe_command, int **pipe_fds, t_env *env)
{
    // Stdin from the appropriate pipe if not the first command!!!
    if(pipe_command->pipe_stdin != STDIN_FILENO)
    {
        if(dup2(pipe_command->pipe_stdin, STDIN_FILENO) == -1)
        {
            perror("dup2 failed on stdin");
            exit(EXIT_FAILURE);
        }
    }

    if(pipe_command->pipe_stdout != STDOUT_FILENO)
    {
        if(dup2(pipe_command->pipe_stdout, STDOUT_FILENO) == -1)
        {
            perror("dup2 failed on stdin");
            exit(EXIT_FAILURE);
        }
    }

    int i = 0;
    while(i < pipe_command->count_pipe -1)
    {
        close(pipe_fds[i][0]); // read end
        close(pipe_fds[i][1]); // write end
        i++;
    }

    if(builtin_check(pipe_command->args))
    {
        run_builtin(pipe_command->args, env);
        exit(env->exit_code);
    } else
    {
        exec_command(pipe_command->args, env, STDOUT_FILENO);
        if(env->exit_code != 0)
            exit(env->exit_code);
        else
            exit(EXIT_FAILURE);
    }
}

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
    if(!pipes)
        return;
    int pipe_count = pipes[0].count_pipe;
    int **pipe_fds = NULL; 
    
    if(pipe_count > 1)
    {
        pipe_fds = setup_pipes(pipes, pipe_count);
        if(!pipe_fds)
        {
            DEBUG_PRINT(RED"no enough pipe!"RESET);
            free_pipe_command(pipes,pipe_count);
            return;
        }
    }



    int i = 0;
    // Fork process for each command
    while(i < pipe_count)
    {
        pipes[i].pipe_id = fork();
        if(pipes[i].pipe_id == -1)
        {
            perror("fork failed");
            continue;
        }
        if(pipes[i].pipe_id == 0)
        {
            if (pipe_count > 1)
                child_process_pipe(&pipes[i],pipe_fds,env);
            else
            {
                if(builtin_check(pipes[i].args))
                {
                    run_builtin(pipes[i].args, env);
                    exit(env->exit_code);
                } else
                {
                    exec_command(pipes[i].args, env, STDOUT_FILENO);
                    if(env->exit_code != 0)
                        exit(env->exit_code);
                    else
                        exit(EXIT_FAILURE);
                }
            
            }
        }
        i++;
    }

    //PArent Process: Close all pipe fds
    if(pipe_count > 1 && pipe_fds)
    {
        i = 0;
        while(i < pipe_count -1)
        {
            close(pipe_fds[i][0]);
            close(pipe_fds[i][1]);
            free(pipe_fds[i]);
            i++;
        }
        free(pipe_fds);
    }

    // Wait for child process!!
    i = 0;
    while(i < pipe_count)
    {
        int status;
        waitpid(pipes[i].pipe_id, &status, 0);
        if(WIFEXITED(status))
            env->exit_code = WEXITSTATUS(status);
        else if(WIFSIGNALED(status))
            env->exit_code = 128 + WTERMSIG(status);
        i++;
    }
    free_pipe_command(pipes,pipe_count);
}