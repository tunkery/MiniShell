/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer_utils1.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bolcay <bolcay@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/24 13:32:04 by bolcay            #+#    #+#             */
/*   Updated: 2025/04/24 14:50:42 by bolcay           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	process_quoted_helper(char **result, char *line, int *i, t_env *env)
{
	char	*temp;

	temp = expand_env(line, i, env);
	if (temp)
	{
		*result = ft_strjoin(*result, temp);
		gc_register(env->s_gc, *result);
	}
}

void	process_quoted_helper2(char **result, char *line, int *i, t_env *env)
{
	char	cpy[2];

	cpy[0] = line[*i];
	cpy[1] = '\0';
	*result = ft_strjoin(*result, cpy);
	gc_register(env->s_gc, *result);
	(*i)++;
}

char	*join_and_register(char *result, char *temp, t_env *env)
{
	result = ft_strjoin(result, temp);
	gc_register(env->s_gc, result);
	return (result);
}

char	*handle_word_helper(char *result, int *i, t_env *env)
{
	char	cpy[2];

	(*i)++;
	cpy[0] = '\\';
	cpy[1] = '\0';
	result = join_and_register(result, cpy, env);
	return (result);
}

int	handle_word_helper2(char **result, char *line, int *i, t_env *env)
{
	char	*temp;

	if (line[*i] == '"')
	{
		temp = process_quoted(line, i, '"', env);
		if (temp)
			*result = join_and_register(*result, temp, env);
		else
			return (-1);
	}
	else if (line[*i] == '\'')
	{
		temp = process_quoted(line, i, '\'', env);
		if (temp)
			*result = join_and_register(*result, temp, env);
		else
			return (-1);
	}
	return (0);
}
