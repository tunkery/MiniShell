/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_heredoc.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hpehliva <hpehliva@student.42heilbronn.de  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/28 11:34:22 by hpehliva          #+#    #+#             */
/*   Updated: 2025/04/28 11:34:34 by hpehliva         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

t_token	*initialized_delimiter(t_token *token, t_env *env)
{
	t_token	*delimiter_token;

	delimiter_token = my_malloc(env->s_gc, sizeof(t_token));
	if (!delimiter_token)
		return (NULL);
	delimiter_token->type = TOKEN_WORD;
	delimiter_token->next = token->next;
	if (token->next)
		token->next->prev = delimiter_token;
	token->next = delimiter_token;
	delimiter_token->prev = token;
	return (delimiter_token);
}

char	*heredoc_quote_check(char *result, char *line, int *i, t_env *env)
{
	char	quote;

	quote = line[*i];
	(*i)++;
	while (line[*i] && line[*i] != quote)
		result = handle_regular_char(result, line, i, env);
	if (line[*i] == '\'' || line[*i] == '"')
		(*i)++;
	return (result);
}

void	parse_delimiter(t_token *token, char *line, int *i, t_env *env)
{
	char	*result;

	result = ft_strdup("");
	gc_register(env->s_gc, result);
	token->quote_mode = 0;
	if (line[*i] == '\'' || line[*i] == '"')
	{
		token->quote_mode = 1;
		result = heredoc_quote_check(result, line, i, env);
	}
	else
	{
		token->quote_mode = 0;
		while (line[*i] && line[*i] != ' ' && line[*i] != '\t'
			&& line[*i] != '|' && line[*i] != '>' && line[*i] != '<'
			&& line[*i] != ';' && line[*i] != '\'' && line[*i] != '"')
			result = handle_regular_char(result, line, i, env);
	}
	token->value = result;
}

void	heredoc_delimiter(t_token *token, char *line, int *i, t_env *env)
{
	t_token	*delimiter_token;

	token->type = TOKEN_HEREDOC;
	token->value = ft_strdup("<<");
	gc_register(env->s_gc, token->value);
	while (line[*i] && (line[*i] == ' ' || line[*i] == '\t'))
		(*i)++;
	delimiter_token = initialized_delimiter(token, env);
	if (delimiter_token)
		return ;
	parse_delimiter(delimiter_token, line, i, env);
}
