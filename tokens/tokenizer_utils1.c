/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer_utils1.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hpehliva <hpehliva@student.42heilbronn.de  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/28 11:35:04 by hpehliva          #+#    #+#             */
/*   Updated: 2025/04/28 11:35:05 by hpehliva         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	*ansi_c_quote(char *str, t_env *env)
{
	char	*result;
	int		i;
	char	escape[2];

	result = ft_strdup("");
	i = 0;
	gc_register(env->s_gc, result);
	while (str[i])
	{
		if (str[i] == '\\')
		{
			i++;
			if (!str[i])
				break ;
			escape[0] = get_escape(str[i]);
			result = ft_strjoin(result, escape);
			gc_register(env->s_gc, result);
		}
		else
			result = regular_char_decrease(result, str, &i, env);
		i++;
	}
	return (result);
}

char	*process_quoted(char *line, int *i, char quote_type, t_env *env)
{
	char	*result;

	result = ft_strdup("");
	gc_register(env->s_gc, result);
	(*i)++;
	while (line[*i] && line[*i] != quote_type)
	{
		if (quote_type == '"' && line[*i] == '$' && line[*i + 1]
			&& (ft_isalnum(line[*i + 1])
				|| line[*i + 1] == '_'
				|| line[*i + 1] == '?'))
			result = handle_quote_var(line, i, result, env);
		else
			result = handle_regular_char(result, line, i, env);
	}
	if (line[*i] == quote_type)
		(*i)++;
	else
		return (NULL);
	return (result);
}

t_token	*handle_special_token(char *line, int *i, t_env *env)
{
	t_token	*token;

	token = my_malloc(env->s_gc, sizeof(t_token));
	if (!token)
	{
		return (NULL);
	}
	token->next = NULL;
	token->prev = NULL;
	if (line[*i] == '|')
		handle_pipe(token, i, env);
	else if (line[*i] == '<')
		handle_redirect_in(token, line, i, env);
	else if (line[*i] == '>')
		handle_redirect_out(token, line, i, env);
	else if (line[*i] == ';')
		handle_semic(token, i, env);
	else
		handle_word(token, line, i, env);
	if (!token->value)
		return (NULL);
	token->next = NULL;
	return (token);
}

void	skip_whitespace(char *line, int *i)
{
	while (line[*i] && (line[*i] == ' ' || line[*i] == '\t'))
		(*i)++;
}

t_token	*add_token_list(t_token *token, t_token **head, t_token *current)
{
	token->prev = current;
	token->next = NULL;
	if (!*head)
		*head = token;
	else
		current->next = token;
	current = token;
	return (token);
}
