/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bolcay <bolcay@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/20 15:53:37 by hpehliva          #+#    #+#             */
/*   Updated: 2025/04/24 13:31:44 by bolcay           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	ft_strcmp(const char *s1, const char *s2)
{
	while (*s1 && *s2 && *s1 == *s2)
	{
		s1++;
		s2++;
	}
	return ((unsigned char)*s1 - (unsigned char)*s2);
}

t_token	*handle_special_token(char *line, int *i, t_env *env)
{
	t_token	*token;

	token = my_malloc(env->s_gc, sizeof(t_token));
	if (!token)
		return (NULL);
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

void	seperated_token(char *line, t_token **head, t_env *env)
{
	t_token	*current;
	t_token	*token;
	int		i;

	i = 0;
	current = NULL;
	while (line[i])
	{
		while (line[i] && (line[i] == ' ' || line[i] == '\t'))
			i++;
		if (line[i] == '\0')
			break ;
		token = handle_special_token(line, &i, env);
		if (!token)
			return ;
		token->prev = current;
		if (!*head)
			*head = token;
		else
			current->next = token;
		current = token;
	}
}

char	*extract_word( char *line, int *i, t_env *env)
{
	char	*result;
	char	cpy[2];

	result = ft_strdup("");
	gc_register(env->s_gc, result);
	while (line[*i] && line[*i] != ' ' && line[*i] != '|' && line[*i] != '<'
		&& line[*i] != '>' && line[*i] != '\'' && line[*i] != '"'
		&& line[*i] != ';')
	{
		cpy[0] = line[*i];
		cpy[1] = '\0';
		result = ft_strjoin(result, cpy);
		gc_register(env->s_gc, result);
		(*i)++;
	}
	return (result);
}
