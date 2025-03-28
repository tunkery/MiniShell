/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hpehliva <hpehliva@student.42heilbronn.de  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/28 14:56:26 by hpehliva          #+#    #+#             */
/*   Updated: 2025/03/28 14:56:28 by hpehliva         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	expanded_heredoc_env(char *line, int *i, char **result)
{
	char	*temp;
	char	cpy[2];

	cpy[0] = line[*i];
	cpy[1] = '\0';
	if (line[*i] == '$')
	{
		temp = expand_env(line, i);
		if (temp)
		{
			*result = ft_strjoin(*result, temp);
			free(temp);
		}
	}
	else
	{
		*result = ft_strjoin(*result, cpy);
		(*i)++;
	}
	return (*i);
}

char	*process_heredoc_line(char *line, char *result)
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
			expand_line = expanded_heredoc_line(&line[i]);
			result = ft_strjoin(result, expand_line);
			free(expand_line);
			i = start + ft_strlen(&line[start]);
		}
		else
		{
			cpy[0] = line[i];
			result = ft_strjoin(result, cpy);
			i++;
		}
	}
	return (ft_strjoin(result, "\n"));
}
