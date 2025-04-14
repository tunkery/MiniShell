/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hpehliva <hpehliva@student.42heilbronn.de  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 13:33:50 by hpehliva          #+#    #+#             */
/*   Updated: 2025/04/14 13:33:52 by hpehliva         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

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

t_token **seg_alloc(t_token *tokens, int seg_count)
{
    t_token **segments = malloc(sizeof(t_token *) * (seg_count + 1));
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

t_token **find_pipe_seg(t_token *tokens, int *seg_count)
{
    *seg_count = count_pipe_seg(tokens);

    return seg_alloc(tokens,*seg_count);
}

int count_args_seg(t_token *start,t_token *end)
{
    int count = 0;
    t_token *tmp = start;
    
     // Count the number of word tokens that aren't redirection targets
    while (tmp && tmp != end && tmp->type != TOKEN_PIPE) {
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

char **args_from_token_alloc(t_token *start, t_token *end, int count)
{
    char **args = malloc(sizeof(char *) * (count + 1));
    if (!args) return NULL;
    
    // Fill the args array
    int i = 0;
    t_token *tmp = start;
    while (tmp && tmp != end && tmp->type != TOKEN_PIPE) {
        if (tmp->type == TOKEN_WORD && 
            !(tmp->prev && (tmp->prev->type == TOKEN_REDIRECT_IN || 
                           tmp->prev->type == TOKEN_REDIRECT_OUT || 
                           tmp->prev->type == TOKEN_REDIRECT_APPEND || 
                           tmp->prev->type == TOKEN_HEREDOC))) {
            args[i++] = ft_strdup(tmp->value);
        }
        tmp = tmp->next;
    }
    args[i] = NULL;
    
    return args;
}
