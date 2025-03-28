/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hpehliva <hpehliva@student.42heilbronn.de  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/24 12:07:05 by hpehliva          #+#    #+#             */
/*   Updated: 2025/03/24 12:07:05 by hpehliva         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static char	*ft_strjoin_free(char *s1, char *s2)
{
	char	*result;

	result = ft_strjoin(s1, s2);
	free(s2);
	return (result);
}

static char	*expand_command_substitution(char *line, int *index)
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
	temp = expand_env(expand_line, &i); // It was about j
	return (temp);
}
/* This functions helps for expanded heredoc */

char	*expanded_heredoc_line(char *line)
{
	char	*result;
	char	*temp;
	int		i;

	DEBUG_PRINT(CYAN "Starting expanded_heredoc_line with input: '%s'\n" RESET,
		line);
	result = ft_strdup("");
	i = 0;
	while (line[i])
	{
		if (line[i] == '$' && line[i + 1] == '(')
		{
			DEBUG_PRINT(CYAN "Found command substitution at index %d\n" RESET,
				i);
			temp = expand_command_substitution(line, &i);
			if (temp)
			{
				DEBUG_PRINT(CYAN "Expanded command substitution: '%s'\n" RESET,
					temp);
				result = ft_strjoin_free(result, temp);
			}
		}
		else
		{
			DEBUG_PRINT(CYAN "Expanding environment variable or copying character at index :%d\n" RESET, i);
			i = expanded_heredoc_env(line, &i, &result);
		}
	}
	DEBUG_PRINT(CYAN "Final expanded line: '%s'\n" RESET, result);
	return (result);
}

/*
	24.03.2025 Heredoc duzgun calismiyordu. Cevre degiskenlerini genisletecek bir yardimci fonksiyon eklemek lazim.
*/
char	*handler_heredoc(char *delimiter)
{
	char	*line;
	char	*result;

	result = ft_strdup("");
	// char *expand_line;
	DEBUG_PRINT(CYAN "Starting heredoc handler for delimiter: '%s'\n" RESET,
		delimiter);
	while (1)
	{
		line = readline("> ");
		if (!line)
		{
			DEBUG_PRINT(CYAN "Heredoc delimiter reached\n" RESET);
			break ;
		}
		if (ft_strcmp(line, delimiter) == 0)
		{
			DEBUG_PRINT(CYAN "Heredoc delimiter : '%s' reached! \n" RESET,
				delimiter);
			free(line);
			break ;
		}
		result = process_heredoc_line(line, result);
		free(line);
	}
	return (result);
}
