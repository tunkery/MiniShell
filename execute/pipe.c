/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: batuhan <batuhan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/13 16:02:31 by hpehliva          #+#    #+#             */
/*   Updated: 2025/04/16 11:35:35 by batuhan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"


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
        // close(pipes[i-1][1]);// close write end of prev pipe
    }
    if(i < seg_count - 1)
    {
        *out_fd = pipes[i][1];
        // close(pipes[i][0]);
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
            clean_2d(args);//free(args); I'm changing instead of free!
        exit(EXIT_FAILURE);
    }
    // Execute the command
    if(builtin_check(args))
    {
        run_builtin(args,env);
        clean_2d(args); // Be sure!
        exit(env->exit_code);
    }
    else
    {
        exec_command(args,env,STDOUT_FILENO);
        clean_2d(args); // Be sure!
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



// Main function to execute piped commands
void execute_piped_command(t_token *tokens, t_env *env)
{
    int seg_count = 0;
    t_token **segments = find_pipe_seg(tokens, &seg_count);
    
    if (!segments || seg_count <= 0) {
        return;
    }
    
    set_signal_pipe();


    if(!preprocess_heredocs(segments,seg_count,env))
    {
         free(segments);
         return;
     }

    // Create pipes
    int **pipes = create_pipes(seg_count);
    if (!pipes && seg_count > 1) 
    {
        free(segments);
        return;
    }
    
    // Create array for process IDs
    pid_t *pids = malloc(sizeof(pid_t)* seg_count);
    if(!pids)
    {
        if(pipes)
            cleanup_pipes(pipes, seg_count);
        free(segments);
        return ;
    }
    
    // Fork processes for each segment
    if(!fork_cmd_process(segments,seg_count,env,pipes,pids))
    {
        if(pipes)
            cleanup_pipes(pipes,seg_count);
        free(pids);
        free(segments);
        return ;
    }

    if(pipes)
        cleanup_pipes(pipes,seg_count);


    wait_child_pipes(pids,seg_count, env);

    
    free(pids);
    free(segments);
}
