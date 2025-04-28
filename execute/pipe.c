/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: batuhan <batuhan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/13 16:02:31 by hpehliva          #+#    #+#             */
/*   Updated: 2025/04/22 19:02:54 by batuhan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"


void find_seg_redirect(int *fds, t_token *start, t_token *end, t_env *env)
{
    int redirec_fds[2];

    redirec_fds[0] = fds[0];
    redirec_fds[1] = fds[1];

    apply_redirections(start,end,redirec_fds, env);
    if(redirec_fds[0] != fds[0])
    {
        if(fds[0] != STDIN_FILENO)
            close(fds[0]);
        fds[0] = redirec_fds[0];
    }

    if(redirec_fds[1] != fds[1])
    {
        if(fds[1] != STDOUT_FILENO)
            close(fds[1]);
        fds[1] = redirec_fds[1];
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
    }
    if(i < seg_count - 1)
    {
        *out_fd = pipes[i][1];
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
    int fds[2];
    char **args;

    setup_child_pipes(pipes,i,seg_count,&fds[0],&fds[1]);

    find_seg_redirect(fds,seg_start,seg_end,env);

    if(!setup_io(fds[0],fds[1]))
        exit(EXIT_FAILURE);
    
    // We can seperate this part!!
    args = create_args_from_tokens(seg_start,seg_end, env);
    if(!args || !args[0])
    {
        // if(args)
        //     clean_2d(args);//free(args); I'm changing instead of free!
        exit(EXIT_FAILURE);
    }
    if(builtin_check(args))
    {
        run_builtin(args,env);
        // clean_2d(args); // Be sure!
        exit(env->exit_code);
    }
    else
    {
        exec_command(args,env,STDOUT_FILENO);
        // clean_2d(args); // Be sure!
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



void execute_piped_command(t_token *tokens, t_env *env)
{
    int seg_count = 0;
    t_token **segments = find_pipe_seg(tokens, &seg_count, env);
    
    if (!segments || seg_count <= 0) {
        return;
    }
    
    set_signal_pipe();


    if(!preprocess_heredocs(segments,seg_count,env))
    {
        //  free(segments);
         return;
     }


    int **pipes = create_pipes(seg_count);
    if (!pipes && seg_count > 1) 
    {
        // free(segments);
        return;
    }
    
    // Create array for process IDs
    pid_t *pids = my_malloc(env->s_gc, sizeof(pid_t)* seg_count);
    if(!pids)
    {
        if(pipes)
            cleanup_pipes(pipes, seg_count);
        // free(segments);
        return ;
    }
    
    if(!fork_cmd_process(segments,seg_count,env,pipes,pids))
    {
        if(pipes)
            cleanup_pipes(pipes,seg_count);
        // free(pids);
        // free(segments);
        return ;
    }

    if(pipes)
        cleanup_pipes(pipes,seg_count);


    wait_child_pipes(pids,seg_count, env);

    
    // free(pids);
    // free(segments);
}
