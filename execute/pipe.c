#include "../minishell.h"

void    free_pipe_command(t_pipe_command *pipes, int pipe_count)
{
    if(!pipes)
        return;
    int i = 0;
    while (i < pipe_count)
    {
        clean_2d(pipes[i].args);
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
            {
                if(tmp->next)
                    end = tmp->next;
                else
                    end = tmp;
            }
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
            pipes[i].args = tokens_to_args(start);
            if(!pipes[i].args)
            {
                free_pipe_command(pipes, *pipe_count);
                return NULL;
            }            
            if(tmp->type == TOKEN_PIPE)
                start = tmp->next;
            else
                start = NULL;
            i++;
        }
        tmp = tmp->next;
    }
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

static void child_process_pipe(t_pipe_command *pipes, int **pipe_fds, t_env *env)
{
    int pipe_count = pipes[0].count_pipe;
    if(pipe_count == 1)
    {
        exec_command(pipes[0].args, env, STDOUT_FILENO);
        exit(env->exit_code);
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
            if(pipes[i].pipe_stdin != STDIN_FILENO)
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

static void main_process(t_pipe_command *pipes, int **pipe_fds, int pipe_count, t_env *env)
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
            env->exit_code = WEXITSTATUS(status);
        else if (WIFSIGNALED(status))
            env->exit_code = 128 + WTERMSIG(status);
        i++;
    }
    free_pipe_command(pipes, pipe_count);
}
void execute_pipes(t_pipe_command *pipes, t_env *env)
{
    int pipe_count = pipes[0].count_pipe;
    int **pipe_fds = setup_pipes(pipes, pipe_count);
    child_process_pipe(pipes, pipe_fds, env);
    main_process(pipes, pipe_fds, pipe_count, env);
}