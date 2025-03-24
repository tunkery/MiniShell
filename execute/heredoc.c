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

char *expanded_heredoc_line(char *line)
{
    char *result = ft_strdup(""); // empty string
    char *temp;
    int i = 0;

    DEBUG_PRINT(CYAN"expanding line : '%s' \n"RESET, line);
    while(line[i])
    {
        if(line[i] == '$')
        {
            temp = expand_env(line, &i);
            if(temp)
                result = ft_strjoin(result, temp);
            free(temp);
        }
        else
        {
            char cpy[2] = {line[i], '\0'};
            result = ft_strjoin(result, cpy);
            i++;
        }
    }
    DEBUG_PRINT(CYAN"Expanded line: '%s'\n"RESET, result);
    return (result);
}


/*
    24.03.2025 Heredoc duzgun calismiyordu. Cevre degiskenlerini genisletecek bir yardimci fonksiyon eklemek lazim.
*/
char *handler_heredoc(char *delimiter)
{
    char *line;
    char *result = ft_strdup("");
    char *expand_line;

    DEBUG_PRINT(CYAN"Starting heredoc handler for delimiter: '%s'\n"RESET, delimiter);
    while(1)
    {
        line = readline("> ");
        if(!line)
        {
            DEBUG_PRINT(CYAN"Heredoc delimiter reached\n"RESET);
            break;
        }
        if(ft_strcmp(line, delimiter) == 0)
        {
            DEBUG_PRINT(CYAN"Heredoc delimiter : '%s' reached! \n"RESET, delimiter);
            free(line);
            break;
        }
        expand_line = expanded_heredoc_line(line);
        result = ft_strjoin(result, line);
        result = ft_strjoin(result, "\n");
        free(line);
    }
    return (result);
}
