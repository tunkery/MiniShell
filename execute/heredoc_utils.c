/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: batuhan <batuhan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/28 14:56:26 by hpehliva          #+#    #+#             */
/*   Updated: 2025/04/22 16:35:34 by batuhan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	*ft_strjoin_heredoc(char const *s1, char const *s2)
{
	int		i;
	int		j;
	char	*new_s;

	if (!s1 || !s2)
		return (NULL);
	i = 0;
	j = 0;
	new_s = malloc (ft_strlen(s1) + ft_strlen(s2) + 1);
	if (!new_s)
		return (NULL);
	while (s1[i])
	{
		new_s[i] = s1[i];
		i++;
	}
	while (s2[j])
	{
		new_s[i + j] = s2[j];
		j++;
	}
	new_s[i + j] = '\0';
	free((void*)s1);
	return (new_s);
}

int	expanded_heredoc_env(char *line, int *i, char **result, t_env *env)
{
	char	*temp;
	char	cpy[2];

	cpy[0] = line[*i];
	cpy[1] = '\0';
	if (line[*i] == '$')
	{
		temp = expand_env(line, i, env);
		if (temp)
		{
			*result = ft_strjoin_heredoc(*result, temp);
			gc_register(env->s_gc, *result);
		}
	}
	else
	{
		*result = ft_strjoin_heredoc(*result, cpy);
		gc_register(env->s_gc, *result);
		(*i)++;
	}
	return (*i);
}

char	*process_heredoc_line(char *line, char *result, t_env *env)
{
	char	*expand_line;
	int		i;
	int		start;
	char	cpy[2];

	i = 0;
	cpy[1] = '\0';
	while (line[i])
	{
		if (line[i] == '$')
		{
			start = i;
			expand_line = expanded_heredoc_line(&line[i], env);
			result = ft_strjoin_heredoc(result, expand_line);
			gc_register(env->s_gc, result);
			i = start + ft_strlen(&line[start]);
		}
		else
		{
			cpy[0] = line[i];
			result = ft_strjoin_heredoc(result, cpy);
			gc_register(env->s_gc, result);
			i++;
		}
	}
	return (ft_strjoin_heredoc(result, "\n"));
}
