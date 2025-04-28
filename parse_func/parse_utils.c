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

int	count_pipe_seg(t_token *tokens)
{
	t_token	*tmp;
	int		count;

	tmp = tokens;
	count = 0;
	while (tmp && tmp->type != TOKEN_SEMIC)
	{
		if (tmp->type == TOKEN_PIPE)
			count++;
		tmp = tmp->next;
	}
	count++;
	return (count);
}

t_token	**seg_alloc(t_token *tokens, int seg_count, t_env *env)
{
	t_token	**segments;
	t_token	*tmp;
	int		i;

	segments = my_malloc(env->s_gc, sizeof(t_token *) * (seg_count + 1));
	if (!segments)
		return (NULL);
	tmp = tokens;
	i = 0;
	segments[i++] = tmp;
	while (tmp && tmp->type != TOKEN_SEMIC)
	{
		if (tmp->type == TOKEN_PIPE)
		{
			segments[i++] = tmp->next;
		}
		tmp = tmp->next;
	}
	segments[i] = NULL;
	return (segments);
}

t_token	**find_pipe_seg(t_token *tokens, int *seg_count, t_env *env)
{
	*seg_count = count_pipe_seg(tokens);
	return (seg_alloc(tokens, *seg_count, env));
}

int	count_args_seg(t_token *start, t_token *end)
{
	int		count;
	t_token	*tmp;

	count = 0;
	tmp = start;
	while (tmp && tmp != end && tmp->type != TOKEN_PIPE)
	{
		if (tmp->type == TOKEN_HEREDOC_PROCESSED || (tmp->prev
				&& tmp->prev->type == TOKEN_HEREDOC_PROCESSED) || (tmp->prev
				&& (tmp->prev->type == TOKEN_HEREDOC)))
		{
			tmp = tmp->next;
			continue ;
		}
		if (tmp->type == TOKEN_WORD && !(tmp->prev
				&& (tmp->prev->type == TOKEN_REDIRECT_IN
					|| tmp->prev->type == TOKEN_REDIRECT_OUT
					|| tmp->prev->type == TOKEN_REDIRECT_APPEND
					|| tmp->prev->type == TOKEN_HEREDOC)))
			count++;
		tmp = tmp->next;
	}
	return (count);
}
