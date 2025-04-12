#include "../minishell.h"

int **create_pipes(int seg_count)
{
    int **pipes = malloc(sizeof(int *) * (seg_count - 1));
    int i = 0;
    int j = 0;
    if (!pipes)
        return NULL;

    
    while (i < seg_count - 1) 
    {
        pipes[i] = malloc(sizeof(int) * 2);
        if (!pipes[i]) {
            j = 0;
            while (j < i) {
                free(pipes[j]);
                j++;
            }
            free(pipes);
            return NULL;
        }
        
        if (pipe(pipes[i]) == -1) {
            perror("pipe failed");
            j = 0;
            while (j < i)
            {
                close(pipes[j][0]);
                close(pipes[j][1]);
                free(pipes[j]);
                j++;
            }
            free(pipes[i]);
            free(pipes);
            return NULL;
            
        }
        i++;
    }
    return pipes;
}

void cleanup_pipes(int **pipes, int seg_count)
{
    int i = 0;
    while(i < seg_count - 1)
    {
        close(pipes[i][0]);
        close(pipes[i][1]);
        free(pipes[i]);
        i++;
    }
    free(pipes);
}

void find_seg_redirect(int *in_fd, int *out_fd, t_token *start, t_token *end, t_env *env)
{
    int redirec_in;
    int redirec_out;

    redirec_in = *in_fd;
    redirec_out = *out_fd;

    apply_redirections(start,end,&redirec_in,&redirec_out, env);
    if(redirec_in != *in_fd)
    {
        if(*in_fd != STDIN_FILENO)
            close(*in_fd);
        *in_fd = redirec_in;
    }

    if(redirec_out != *out_fd)
    {
        if(*out_fd != STDOUT_FILENO)
            close(*out_fd);
        *out_fd = redirec_out;
    }
    
}

void setup_child_pipes(int **pipes, int i, int seg_count, int *in_fd, int *out_fd)
{
    *in_fd = STDIN_FILENO;
    *out_fd = STDOUT_FILENO;
    int j = 0;
    if(i > 0)
    {
        *in_fd = pipes[i-1][0];
        close(pipes[i-1][1]);// close write end of prev pipe
    }
    if(i < seg_count - 1)
    {
        *out_fd = pipes[i][1];
        close(pipes[i][0]);
    }

    while(j < seg_count - 1)
    {
        if(j != i -1)
            close(pipes[j][0]);
        if(j != i)
            close(pipes[j][1]);
        j++;
    }


}

int setup_io(int in_fd, int out_fd)
{
    if(in_fd != STDIN_FILENO)
    {
        if(dup2(in_fd,STDIN_FILENO) == -1)
        {
            perror("dup2 for stdin failed");
            return 0;
        }
        close(in_fd);
    }

    if(out_fd != STDOUT_FILENO)
    {
        if(dup2(out_fd,STDOUT_FILENO) == -1)
        {
            perror("dup2 for stdout failed");
            return 0;
        }
        close(out_fd);
    }
    return 1;
}

void exec_child_comd(t_token *seg_start, t_token *seg_end, t_env *env, int **pipes, int i, int seg_count)
{
    int in_fd;
    int out_fd;
    char **args;

    setup_child_pipes(pipes,i,seg_count,&in_fd,&out_fd);

    // Apply seg redirection
    find_seg_redirect(&in_fd,&out_fd,seg_start,seg_end,env);

    if(!setup_io(in_fd,out_fd))
        exit(EXIT_FAILURE);
    
    // We can seperate this part!!
    args = create_args_from_tokens(seg_start,seg_end);
    if(!args || !args[0])
    {
        if(args)
            free(args);
        exit(EXIT_FAILURE);
    }

    // Execute the command
    if(builtin_check(args))
    {
        run_builtin(args,env);
        exit(env->exit_code);
    }
    else
    {
        exec_command(args,env,STDOUT_FILENO);
        exit(env->exit_code);
    }
}

int fork_cmd_process(t_token **segments, int seg_count, t_env *env, int **pipes, pid_t *pids)
{
    int i = 0;
    t_token *seg_end;
    while(i < seg_count)
    {
        pids[i] = fork();
        if(pids[i] < 0)
        {
            perror("fork failed");
            return 0;
        }

        if(pids[i] == 0)
        {
            if(i < seg_count - 1)
                seg_end = segments[i + 1]->prev;
            else
                seg_end = NULL;
            exec_child_comd(segments[i],seg_end,env,pipes,i,seg_count);
            exit(EXIT_FAILURE);
        }
        i++;
    }
    return 1;
}

void wait_child_pipes(pid_t *pids, int seg_count, t_env *env)
{
    int i = 0;
    int status;
    while(i < seg_count)
    {
        waitpid(pids[i],&status,0);

        // Store the exit code of the last process
        if(i == seg_count - 1)
        {
            if(WIFEXITED(status))
                env->exit_code = WEXITSTATUS(status);
            else if(WIFSIGNALED(status))
                env->exit_code = 128 + WTERMSIG(status);
        }
        i++;
    }
}

// Main function to execute piped commands
void execute_piped_command(t_token *tokens, t_env *env)
{
    int seg_count = 0;
    t_token **segments = find_pipe_seg(tokens, &seg_count);
    
    if (!segments || seg_count <= 0) {
        DEBUG_PRINT(RED "Failed to identify pipe segments\n" RESET);
        return;
    }
    
    // Create pipes
    int **pipes = create_pipes(seg_count);
    if (!pipes) 
    {
        free(segments);
        return;
    }
    
    // Create array for process IDs
    pid_t *pids = malloc(sizeof(pid_t)* seg_count);
    if(!pids)
    {
        cleanup_pipes(pipes, seg_count);
        free(segments);
        return ;
    }
    
    // Fork processes for each segment
    if(!fork_cmd_process(segments,seg_count,env,pipes,pids))
    {
        cleanup_pipes(pipes,seg_count);
        free(pids);
        free(segments);
        return ;
    }

    cleanup_pipes(pipes,seg_count);


    wait_child_pipes(pids,seg_count, env);

    
    free(pids);
    free(segments);
}