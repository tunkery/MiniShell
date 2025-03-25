/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exe_with_token.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hpehliva <hpehliva@student.42heilbronn.de  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/25 14:24:26 by hpehliva          #+#    #+#             */
/*   Updated: 2025/03/25 14:24:28 by hpehliva         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void    process_child_heredoc(t_token **current, char **heredoc_input, char **args, t_env *env)
{
    if(*current && (*current)->type == TOKEN_WORD)
    {
        int pipe_fd[2];
        pid_t pid;

        if(pipe(pipe_fd) == -1)
        {
            perror("pipe failed!");
            clean_2d(args);
            return ;
        } // Surecler arasi boruyu olusturur > alt surec icin
        pid = fork();
        if(pid == 0)
        {
            close(pipe_fd[0]);
            *heredoc_input = handler_heredoc((*current)->value, env);
            write(pipe_fd[1], *heredoc_input, ft_strlen(*heredoc_input));
            close(pipe_fd[1]);
            free(*heredoc_input);
            exit(0);
        }
        else if(pid > 0)
        {
            close(pipe_fd[1]);
            dup2(pipe_fd[0], STDIN_FILENO);
            close(pipe_fd[0]);
            wait(NULL); // Alt sureci bekle
        }
        else
        {
            perror("fork failed!");
            clean_2d(args);
            return ;
        }
        DEBUG_PRINT(GRN"HEredoc process for delimiter '%s'\n"RESET, (*current)->value);
    }
}

void openfile_redirected(t_token **current, int *out_fd, char **args, int append)
{
    int flag;
    flag = O_WRONLY | O_CREAT;
    if(append)
        flag |= O_APPEND;
    else
        flag |= O_TRUNC;

    if(*current && (*current)->type == TOKEN_WORD)
    {
        *out_fd = open((*current)->value, flag, 0644); // Uzerine yazdiriyor.
        if(out_fd < 0)
        {
            perror("open failed!");
            clean_2d(args);
            return ;
        }
        DEBUG_PRINT(BLUE"Redirecting output to '%s'\n"RESET, (*current)->value);
    }
}