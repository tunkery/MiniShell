/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_utils1.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bolcay <bolcay@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/24 15:26:56 by bolcay            #+#    #+#             */
/*   Updated: 2025/04/24 15:37:30 by bolcay           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	count_args_seg(t_token *start, t_token *end)
{
	int		count;
	t_token	*tmp;

	count = 0;
	tmp = start;
	while (tmp && tmp != end && tmp->type != TOKEN_PIPE)
	{
		if (tmp->type == TOKEN_HEREDOC_PROCESSED || (tmp->prev
				&& tmp->prev->type == TOKEN_HEREDOC_PROCESSED)
			|| (tmp->prev && (tmp->prev->type == TOKEN_HEREDOC)))
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

t_token	**find_pipe_seg(t_token *tokens, int *seg_count, t_env *env)
{
	*seg_count = count_pipe_seg(tokens);
	return (seg_alloc(tokens, *seg_count, env));
}

int	token_alloc_check1(t_token *tmp)
{
	if (tmp->type == TOKEN_WORD && !(tmp->prev
			&& (tmp->prev->type == TOKEN_REDIRECT_IN
				|| tmp->prev->type == TOKEN_REDIRECT_OUT
				|| tmp->prev->type == TOKEN_REDIRECT_APPEND
				|| tmp->prev->type == TOKEN_HEREDOC)))
	{
		return (-1);
	}
	return (0);
}

int	token_alloc_check(t_token *tmp)
{
	if (tmp->type == TOKEN_HEREDOC_PROCESSED || (tmp->prev
			&& tmp->prev->type == TOKEN_HEREDOC_PROCESSED)
		|| (tmp->prev && (tmp->prev->type == TOKEN_HEREDOC)))
	{
		return (-1);
	}
	return (0);
}
