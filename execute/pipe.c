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

int create_single_process(t_token **segments, int i, t_pipe_seg *stat)
{
    t_token *seg_end;

    stat->seg_index = i;

    if(i < stat->seg_count - 1)
        seg_end = segments[i + 1]->prev;
    else
        seg_end = NULL;
    stat->seg_start = segments[i];
    stat->seg_end = seg_end;
    return 1;
}


int fork_cmd_process(t_token **segments, int seg_count, t_env *env, int **pipes)
{
    pid_t *pids = env->pids;
    t_pipe_seg stat;

    init_pipe_seg(&stat,pipes, seg_count, 0);
    int i = 0;
    while(i < seg_count)
    {
        if(!create_single_process(segments,i,&stat))
            return 0;
        pids[i] = fork();
        if(pids[i] < 0)
        {
            perror("fork failed");
            return 0;
        }

        if(pids[i] == 0)
        {
            exec_child_comd(&stat, env);
        }
        i++;
    }
    return 1;
}

int setup_pipe_exec(t_token **segments, int seg_count, t_env *env, int ***pipes_ptr)
{

    if(!preprocess_heredocs(segments,seg_count,env))
         return 0;
    *pipes_ptr = create_pipes(seg_count);
    if(!*pipes_ptr && seg_count > 1)
        return 0;
    env->pids = my_malloc(env->s_gc, sizeof(pid_t)* seg_count);
    if(!env->pids)
    {
        if(*pipes_ptr)
            cleanup_pipes(*pipes_ptr, seg_count);
        return 0;
    }

    return 1;
}

void execute_piped_command(t_token *tokens, t_env *env)
{
    int seg_count = 0;
    int **pipes = NULL;
    t_token **segments = find_pipe_seg(tokens, &seg_count, env);
    
    if (!segments || seg_count <= 0)
        return;
    set_signal_pipe();
    if(!setup_pipe_exec(segments,seg_count,env, &pipes))
        return;
    if(!fork_cmd_process(segments,seg_count,env,pipes))
    {
        if(pipes)
            cleanup_pipes(pipes,seg_count);
        return ;
    }
    if(pipes)
        cleanup_pipes(pipes,seg_count);
    wait_child_pipes(env->pids,seg_count, env);
}
