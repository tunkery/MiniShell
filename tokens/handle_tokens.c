/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_tokens.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bolcay <bolcay@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/02 12:35:28 by hpehliva          #+#    #+#             */
/*   Updated: 2025/04/24 14:47:05 by bolcay           ###   ########.fr       */
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
	{
		token->type = TOKEN_HEREDOC;
		token->value = ft_strdup("<<");
		gc_register(env->s_gc, token->value);
		*i += 2;
		while (line[*i] && line[*i] == ' ')
			(*i)++;
	}
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
	char	*temp;
	// char	cpy[2];

	result = ft_strdup("");
	gc_register(env->s_gc, result);
	token->type = TOKEN_WORD;
	while (line[*i] && line[*i] != ' ' && line[*i] != '\t' && line[*i] != '|'
		&& line[*i] != '>' && line[*i] != '<' && line[*i] != ';')
	{
		// Add backslash rules!
		if (line[*i] == '\\')
		{
			(*i)++;
			if (!line[*i])
			{
				token->value = NULL;
				return ;
			}
			if (line[*i] == '\\' && line[*i + 1] == '$')
			{
				result = handle_word_helper(result, i, env);
				continue ;
			}
			process_quoted_helper2(&result, line, i, env);
			continue ;
		}
		if (line[*i] == '"')
		{
			temp = process_quoted(line, i, '"', env);
			if (temp)
				result = join_and_register(result, temp, env);
			else
			{
				token->value = NULL;
				return ;
			}
		}
		else if (line[*i] == '\'')
		{
			temp = process_quoted(line, i, '\'', env);
			if (temp)
				result = join_and_register(result, temp, env);
			else
			{
				token->value = NULL;
				return ;
			}
		}
		else if (line[*i] == '$' && (line[*i + 1] && (ft_isalnum(line[*i + 1])
					|| line[*i + 1] == '_' || line[*i + 1] == '?' || line[*i
						+ 1] == '"')))
			process_quoted_helper(&result, line, i, env);
		else if (line[*i] == '~' && (line[*i + 1] == '/' || line[*i + 1] == '\0'
				|| line[*i + 1] == ' '))
		{
			temp = handle_tilde(line, i, env);
			if (temp)
				result = join_and_register(result, temp, env);
		}
		else
			process_quoted_helper2(&result, line, i, env);
	}
	token->value = result;
}
