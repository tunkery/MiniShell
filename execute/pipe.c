#include "../minishell.h"

// char **split_pipes(char *command)
// {
//     char **args = NULL;
//     char *token;
//     int i = 0;
//     char cpy_cmd = ft_strdup(command);

//     token = ft_strtok(cpy_cmd, "|");
//     while(token != NULL)
//     {
//         args = ft_realloc(args, sizeof(char *) * (i + 1));
//         args[i] = ft_strtrim(token, " ");
//         i++;
//         token = ft_strtok(NULL, "|");
//     }
//     args = ft_realloc(args, sizeof(char *) * (i + 1));
//     args[i] = NULL;
//     free(cpy_cmd);
//     return args;
// }
// char **split_args(char *command)
// {
//     char **args = NULL;
//     char *token;
//     int i = 0;
//     char cpy_cmd = ft_strdup(command);

//     token = ft_strtok(cpy_cmd, " ");
//     while(token != NULL)
//     {
//         args = ft_realloc(args, sizeof(char *) * (i + 1));
//         args[i] = token;
//         i++;
//         token = ft_strtok(NULL, " ");
//     }
//     args = ft_realloc(args, sizeof(char *) * (i + 1));
//     args[i] = NULL;
//     free(cpy_cmd);
//     return args;
// }
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
    *pipe_count = count_pipes(tokens);
    t_pipe_command *pipes = malloc(sizeof(t_pipe_command) * (*pipe_count));
    if (!pipes)
        return NULL;
    int i = 0;
    t_token *start = tokens;
    while (tmp)
    {
        if (tmp->type == TOKEN_PIPE || !tmp->next)
        {
            t_token *end;
            if(tokens->type == TOKEN_PIPE)
                end = tmp;
            else
                end = tmp->next;
            pipes[i].count_pipe = *pipe_count;
            pipes[i].index = i;
            pipes[i].pipe_id = -1;
            if( i == 0)
                pipes[i].pipe_stdin = STDIN_FILENO;
            else
                pipes[i].pipe_stdin = -1;
        
            if(i == *pipe_count - 1)
                pipes[i].pipe_stdout = STDOUT_FILENO;
            else
                pipes[i].pipe_stdout = -1;
            pipes[i].args = tokens_to_args(start, end);
            if(tmp->type == TOKEN_PIPE)
                start = tmp->next;
            else
                start = NULL;
            i++;
        }
        tmp = tmp->next;
    }
    return pipes;

    // char **args = pipe_split(command);
    // int i = 0;
    // while(args[i])
    //    i++;
    // *pipe_count = i;

    // t_pipe_command *pipes = malloc(sizeof(t_pipe_command) * i);
    // if(!pipes)
    //     return NULL;
    
    // i = 0;
    // while(i < *pipe_count)
    // {
    //     pipes[i].count_pipe = *pipe_count;
    //     pipes[i].index = i;
    //     pipes[i].pipe_id = -1;
    //     if( i == 0)
    //         pipes[i].pipe_stdin = STDIN_FILENO;
    //     else
    //         pipes[i].pipe_stdin = -1;
        
    //     if(i == *pipe_count - 1)
    //         pipes[i].pipe_stdout = STDOUT_FILENO;
    //     else
    //         pipes[i].pipe_stdout = -1;
    //     pipes[i].args = split_args(args[i]);
    //     i++;
    // }
    // i = 0;
    // while(args[i])
    // {
    //     free(args[i]);
    //     i++;
    // }
    // return pipes;
}

static int **setup_pipes(t_pipe_command *pipes, int pipe_count)
{
    int **pipe_fds = NULL;

    if (pipe_count == 1)
        return NULL;
    pipe_fds = malloc(sizeof(int *) * (pipe_count - 1));
    if (!pipe_fds)
    {
        perror("malloc");
        exit(EXIT_FAILURE);
    }
    int i = 0;
    while (i < pipe_count - 1)
    {
        pipe_fds[i] = malloc(sizeof(int) * 2);
        if (!pipe_fds[i])
        {
            perror("Malloc failed for pipe_fds[i]");
            exit(EXIT_FAILURE);
        }
        if(pipe(pipe_fds[i]) == -1)
        {
            perror("Pipe failed");
            exit(EXIT_FAILURE);
        }
        pipes[i].pipe_stdout = pipe_fds[i][1];
        pipes[i + 1].pipe_stdin = pipe_fds[i][0];
        i++;
    }
    pipes[0].pipe_stdin = STDIN_FILENO;
    pipes[pipe_count - 1].pipe_stdout = STDOUT_FILENO;
    return pipe_fds;
}

static void child_process_pipe(t_pipe_command *pipes, int **pipe_fds, t_env *env)
{
    int pipe_count = pipes[0].count_pipe;
    if(pipe_count == 1)
    {
        exec_command(pipes[0].args, env, STDOUT_FILENO);
        return;
    }
    int i = 0;
    while(i < pipe_count)
    {
        pipes[i].pipe_id = fork();
        if (pipes[i].pipe_id == -1)
        {
            perror("Fork failed");
            exit(EXIT_FAILURE);
        }
        // Child process
        if(pipes[i].pipe_id == 0)
        {
            if(pipes[i].pipe_stdin != STDIN_FILENO && i == 0 && !isatty(STDIN_FILENO))
            {
                int fd_null = open("/dev/null", O_RDONLY);
                if(fd_null != -1)
                {
                    dup2(fd_null, STDIN_FILENO);
                    close(fd_null);
                }
            }
            else if(pipes[i].pipe_stdin != STDIN_FILENO)
            {
                dup2(pipes[i].pipe_stdin, STDIN_FILENO);
                close(pipes[i].pipe_stdin);
            }
            if(pipes[i].pipe_stdout != STDOUT_FILENO)
            {
                dup2(pipes[i].pipe_stdout, STDOUT_FILENO);
                close(pipes[i].pipe_stdout);
            }
            int j = 0;
            while(j < pipe_count - 1)
            {
                close(pipe_fds[j][0]);
                close(pipe_fds[j][1]);
                j++;
            }
            exec_command(pipes[i].args, env, pipes[i].pipe_stdout);
            exit(env->exit_code);
        }
    }
}

// If the function who call wait_for_child is not being static. We can use it when we clean it the command !

static void main_process(t_pipe_command *pipes, int **pipe_fds, int pipe_count)
{
    int status;

    if(pipe_count == 1)
    {
        clean_2d(pipes[0].args);
        free(pipes);
        return;
    }
    int i =0;
    while(i < pipe_count)
    {
        close(pipe_fds[i][0]);
        close(pipe_fds[i][1]);
        free(pipe_fds[i]);
        i++;
    }
    free(pipe_fds);
    i = 0;
    while(i < pipe_count)
    {
        waitpid(pipes[i].pipe_id, &status, 0);
        if (WIFEXITED(status))
            pipes[0].count_pipe = WEXITSTATUS(status);
        i++;
    }
    i = 0;
    while(i < pipe_count)
    {
        clean_2d(pipes[i].args);
        i++;
    }
    free(pipes);
}
void execute_pipes(t_pipe_command *pipes, t_env *env)
{
    int pipe_count = pipes[0].count_pipe;
    int **pipe_fds = setup_pipes(pipes, pipe_count);
    child_process_pipe(pipes, pipe_fds, env);
    main_process(pipes, pipe_fds, pipe_count);
    env->exit_code = pipes[0].count_pipe;
}