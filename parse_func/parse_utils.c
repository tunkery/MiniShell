/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: batuhan <batuhan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 13:33:50 by hpehliva          #+#    #+#             */
/*   Updated: 2025/04/22 16:46:16 by batuhan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static int parent_process_heredoc_pipe(int *pipe_fd,pid_t pid,t_token *curr, t_env *env)
{
    close(pipe_fd[1]);

    int status;
    waitpid(pid,&status, 0);
    if(WIFSIGNALED(status))
    {
        close(pipe_fd[0]);
        return 0;
    }
    char fd_str[16];
    snprintf(fd_str, sizeof(fd_str),"%d", pipe_fd[0]);

    // free(curr->value);
    curr->value = ft_strdup(fd_str);
    gc_register(env->s_gc, curr->value);
    curr->type = TOKEN_HEREDOC_PROCESSED;
    return 1;
}

int preprocess_heredocs(t_token **seg, int seg_count, t_env *env)
{
    int i = 0;
    t_token *curr;
    t_token *seg_end;
    char *heredoc_input = NULL;
    while(i < seg_count)
    {
        curr = seg[i];
        if(i < seg_count -1)
            seg_end = seg[i+1]->prev;
        else
            seg_end = NULL;
        
        while(curr && curr != seg_end && curr->type != TOKEN_PIPE)
        {
            if(curr->type == TOKEN_HEREDOC && curr->next && curr->next->type == TOKEN_WORD)
            {
                int pipe_fd[2];
                if(pipe(pipe_fd) == -1)
                {
                    perror("pipe failed");
                    return 0;
                }

                pid_t pid = fork();
                if(pid == -1)
                {
                    perror("fork failed");
                    close(pipe_fd[0]);
                    close(pipe_fd[1]);
                    return 0;
                }
                if (pid == 0)
                {
                    close(pipe_fd[0]);
                    set_signal_heredoc();
                    heredoc_input = handler_heredoc(curr->next->value, env);
                    write(pipe_fd[1], heredoc_input,ft_strlen(heredoc_input));
                    close(pipe_fd[1]);
                    // free(heredoc_input);
                    exit(0);
                }
                else
                {
                    if(!parent_process_heredoc_pipe(pipe_fd,pid,curr, env))
                        return 0;

                }
            }
            curr = curr->next;
        }
        i++;
    }
    return 1;
}



// Helper function to check if tokens contain pipe
int count_pipe_seg(t_token * tokens)
{
    t_token *tmp = tokens;
    int count = 0;

    while (tmp && tmp->type != TOKEN_SEMIC) {
        if (tmp->type == TOKEN_PIPE)
            count++;
        tmp = tmp->next;
    }
    count++;

    return count;

}

t_token **seg_alloc(t_token *tokens, int seg_count, t_env *env)
{
    t_token **segments = my_malloc(env->s_gc, sizeof(t_token *) * (seg_count + 1));
    if (!segments)
        return NULL;
    
    // Find start of each segment
    t_token *tmp = tokens;
    int i = 0;
    segments[i++] = tmp; // First segment starts
    
    while (tmp && tmp->type != TOKEN_SEMIC) {
        if (tmp->type == TOKEN_PIPE) {
            segments[i++] = tmp->next; // Next segment starts after pipe
        }
        tmp = tmp->next;
    }
    
    segments[i] = NULL; // End
    
    return segments;
}

t_token **find_pipe_seg(t_token *tokens, int *seg_count, t_env *env)
{
    *seg_count = count_pipe_seg(tokens);

    return seg_alloc(tokens,*seg_count, env);
}

int count_args_seg(t_token *start,t_token *end)
{
    int count = 0;
    t_token *tmp = start;
    
     // Count the number of word tokens that aren't redirection targets
    while (tmp && tmp != end && tmp->type != TOKEN_PIPE) {
        if(tmp->type == TOKEN_HEREDOC_PROCESSED ||(tmp->prev && tmp->prev->type == TOKEN_HEREDOC_PROCESSED))
        {
            tmp = tmp->next;
            continue;
        }
        if(tmp->prev && (tmp->prev->type == TOKEN_HEREDOC))
        {
            tmp = tmp->next;
            continue;
        }
        if (tmp->type == TOKEN_WORD && 
            !(tmp->prev && (tmp->prev->type == TOKEN_REDIRECT_IN || 
                           tmp->prev->type == TOKEN_REDIRECT_OUT || 
                           tmp->prev->type == TOKEN_REDIRECT_APPEND || 
                           tmp->prev->type == TOKEN_HEREDOC))) {
            count++;
        }
        tmp = tmp->next;
    }
    return count;
}

char **args_from_token_alloc(t_token *start, t_token *end, int count, t_env *env)
{
    char **args = my_malloc(env->s_gc, sizeof(char *) * (count + 1));
    if (!args) return NULL;
    
    // Fill the args array
    int i = 0;
    t_token *tmp = start;
    while (tmp && tmp != end && tmp->type != TOKEN_PIPE) {
        if(tmp->type == TOKEN_HEREDOC_PROCESSED ||(tmp->prev && tmp->prev->type == TOKEN_HEREDOC_PROCESSED))
        {
            tmp = tmp->next;
            continue;
        }
        if(tmp->prev && (tmp->prev->type == TOKEN_HEREDOC))
        {
            tmp = tmp->next;
            continue;
        }
        if (tmp->type == TOKEN_WORD && 
            !(tmp->prev && (tmp->prev->type == TOKEN_REDIRECT_IN || 
                           tmp->prev->type == TOKEN_REDIRECT_OUT || 
                           tmp->prev->type == TOKEN_REDIRECT_APPEND || 
                           tmp->prev->type == TOKEN_HEREDOC))) {
            args[i] = ft_strdup(tmp->value);
            gc_register(env->s_gc, args[i++]);
        }
        tmp = tmp->next;
    }
    args[i] = NULL;
    
    return args;
}
