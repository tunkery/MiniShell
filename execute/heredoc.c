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

char *close_heredoc_line(char *expand_line, t_env *env)
{
    int pipe_fd[2];
    pid_t pid;
    char *result = ft_strdup("");
    char *line;
    char *args[] = {expand_line, NULL};
    if(pipe(pipe_fd) == -1)
    {
        perror("pipe failed!");
        return (ft_strdup(""));
    }
    pid = fork();
    if(pid == 0)
    {
        close(pipe_fd[0]);
        dup2(pipe_fd[1], STDOUT_FILENO);
        close(pipe_fd[1]);
        execve(find_exec(expand_line, env->path1, 0, 5), args, env->path);
        exit(127);
    }
    else if(pid > 0)
    {
        close(pipe_fd[1]);
        while((line = get_next_line(pipe_fd[0])) != NULL)
        {
            char *temp = ft_strjoin(result, line);
            free(result);
            result = temp;
            free(line);
        }
        close(pipe_fd[0]);
        wait(NULL);
    }
    else
    {
        perror("fork failed!");
        close(pipe_fd[0]);
        close(pipe_fd[1]);
        return (ft_strdup(""));
    }
    free(env->path);
    free(env);
    return result;
}

char *expanded_heredoc_line(char *line, t_env *env)
{
    char *result = ft_strdup(""); // empty string
    char *temp;
    char expand_line[256];
    int i;
    int j;

    i = 0;
    DEBUG_PRINT(CYAN"expanding line : '%s' \n"RESET, line);
    while(line[i])
    {
        if(line[i] == '$' && line[i+1] == '(')
        {
            i += 2;
            j = 0;
            while(line[i] && line[i] != ')')
                expand_line[j++] = line[i++];
            expand_line[j] = '\0';
            if(line[i] == ')')
                i++;
            DEBUG_PRINT(CYAN"Expanding command substitution: '%s'\n"RESET, expand_line);
            // ADD OUTPUT_HEREDOC
            temp = close_heredoc_line(expand_line, env);
            if(temp)
                result = ft_strjoin(result, temp);
            free(temp);
        }
        else if(line[i] == '$')
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
        i++;
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
        int i = 0;
        while(line[i])
        {
            if(line[i] == '$')
            {
                expand_line = expand_env(line, &i);
                result = ft_strjoin(result, expand_line);
                free(expand_line);
            }
            else
            {
                char cpy[2] = {line[i], '\0'};
                result = ft_strjoin(result, cpy);
            }
            i++;
        }
        result = ft_strjoin(result, line);
        result = ft_strjoin(result, "\n");
        free(line);
    }
    return (result);
}
