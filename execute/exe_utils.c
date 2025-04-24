/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exe_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bolcay <bolcay@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/24 16:35:42 by bolcay            #+#    #+#             */
/*   Updated: 2025/04/24 18:03:09 by bolcay           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	token_to_args_helper(t_token **temp)
{
	t_token	*tmp;

	tmp = *temp;
	if (tmp->type == TOKEN_REDIRECT_APPEND || tmp->type == TOKEN_HEREDOC
		|| tmp->type == TOKEN_REDIRECT_OUT || tmp->type == TOKEN_REDIRECT_IN)
	{
		tmp = tmp->next;
		if (tmp)
			tmp = tmp->next;
		*temp = tmp;
		return (-1);
	}
	return (0);
}

void	token_to_args_helper1(char ***args, t_token *tmp, t_env *env, int *i)
{
	(*args)[*i] = ft_strdup(tmp->value);
	gc_register(env->s_gc, (*args)[*i]);
	(*i)++;
}

int	count_token_helper(t_token *tokens)
{
	if (tokens->type == TOKEN_REDIRECT_APPEND
		|| tokens->type == TOKEN_HEREDOC
		|| tokens->type == TOKEN_REDIRECT_OUT
		|| tokens->type == TOKEN_REDIRECT_IN
		|| tokens->type == TOKEN_HEREDOC)
		return (-1);
	return (0);
}

void	close_both(int save_stdout, int save_stdin)
{
	close(save_stdout);
	close(save_stdin);
}
