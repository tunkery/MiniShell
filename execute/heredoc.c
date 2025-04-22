/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: batuhan <batuhan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/24 12:07:05 by hpehliva          #+#    #+#             */
/*   Updated: 2025/04/15 17:07:41 by batuhan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static char	*ft_strjoin_free(char *s1, char *s2)
{
	char	*result;

	result = ft_strjoin_heredoc(s1, s2);
	free(s2);
	return (result);
}

static char	*expand_command_substitution(char *line, int *index, t_env *env)
{
	char	expand_line[256];
	char	*temp;
	int		i;

	i = 0;
	(*index) += 1;
	while (line[*index] && line[*index] != ')')
		expand_line[i++] = line[(*index)++];
	expand_line[i] = '\0';
	if (line[*index] == ')')
		(*index)++;
	i = 0;
	temp = expand_env(expand_line, &i, env); // It was about j
	return (temp);
}
/* This functions helps for expanded heredoc */

char	*expanded_heredoc_line(char *line, t_env *env)
{
	char	*result;
	char	*temp;
	int		i;

	result = ft_strdup("");
	i = 0;
	while (line[i])
	{
		if (line[i] == '$' && line[i + 1] == '(')
		{
			temp = expand_command_substitution(line, &i, env);
			if (temp)
			{
				result = ft_strjoin_free(result, temp);
			}
		}
		else
		{
			i = expanded_heredoc_env(line, &i, &result, env);
		}
	}
	return (result);
}


/*
	24.03.2025 Heredoc duzgun calismiyordu. Cevre degiskenlerini genisletecek bir yardimci fonksiyon eklemek lazim.
*/
char	*handler_heredoc(char *delimiter, t_env *env,int quote_mode)
{
	char	*line;
	char	*result;

	result = ft_strdup("");
	// char *expand_line;
	while (1)
	{
		line = readline("> ");
		if (!line)
			break ;
		if (ft_strcmp(line, delimiter) == 0)
		{
			free(line);
			break ;
		}
		if(quote_mode)
		{
			char *temp = result;
			result = ft_strjoin(result,line);
			free(temp);

			temp = result;
			result = ft_strjoin(result, "\n");
			free(temp);
		}
		else
			result = process_heredoc_line(line, result, env);
		free(line);
	}
	return (result);
}
