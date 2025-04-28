/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_tokens.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: batuhan <batuhan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/02 12:35:28 by hpehliva          #+#    #+#             */
/*   Updated: 2025/04/22 16:15:00 by batuhan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	handle_pipe(t_token *token, int *i, t_env *env)
{
	token->type = TOKEN_PIPE;
	token->value = ft_strdup("|");
	gc_register(env->s_gc, token->value);
	(*i)++;
}

void	handle_redirect_in(t_token *token, char *line, int *i, t_env *env)
{
	if (line[*i + 1] == '<')
		heredoc_delimiter(token, line, i, env);
	else
	{
		token->type = TOKEN_REDIRECT_IN;
		token->value = ft_strdup("<");
		gc_register(env->s_gc, token->value);
		(*i)++;
	}
}

void	handle_redirect_out(t_token *token, char *line, int *i, t_env *env)
{
	if (line[*i + 1] == '>')
	{
		token->type = TOKEN_REDIRECT_APPEND;
		token->value = ft_strdup(">>");
		gc_register(env->s_gc, token->value);
		(*i) += 2;
	}
	else
	{
		token->type = TOKEN_REDIRECT_OUT;
		token->value = ft_strdup(">");
		gc_register(env->s_gc, token->value);
		(*i)++;
	}
}

void	handle_semic(t_token *token, int *i, t_env *env)
{
	token->type = TOKEN_SEMIC;
	token->value = ft_strdup(";");
	gc_register(env->s_gc, token->value);
	(*i)++;
}

void	handle_word(t_token *token, char *line, int *i, t_env *env)
{
	char	*result;

	result = ft_strdup("");
	gc_register(env->s_gc, result);
	token->type = TOKEN_WORD;
	result = process_word_token(result, line, i, env);
	if (!result)
	{
		token->value = NULL;
		return ;
	}
	token->value = result;
}
