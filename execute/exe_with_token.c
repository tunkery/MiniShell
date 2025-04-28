/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exe_with_token.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hpehliva <hpehliva@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/25 14:24:26 by hpehliva          #+#    #+#             */
/*   Updated: 2025/04/28 23:12:28 by hpehliva         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"


void	read_redirected_in(t_token **current, int *in_fd, char **args, t_env *env)
{
	if(*current && (*current)->type == TOKEN_WORD)
	{
		*in_fd = open((*current)->value, O_RDONLY);
		if(*in_fd < 0)
		{
			env->exit_code = 1;
			write(2, "minishell: ", 11);
			write(2, (*current)->value,ft_strlen((*current)->value));
			write(2,": No such file or directory\n", 29);
			*args = NULL;
			return;
		}
		*current = (*current)->next;
	}
	if(*in_fd  != STDIN_FILENO)
	{
		if(dup2(*in_fd, STDIN_FILENO) == -1)
		{
			env->exit_code = 1;
			close(*in_fd);
			perror("dub2 failed for redirected!");
			*args = NULL;
			return ;
		}
		close(*in_fd);
	}
}



void	process_child_heredoc(t_token **current, char **heredoc_input,
	char **args, t_env *env)
{
	int pipe_fd[2];
	pid_t pid;

	if (!*current && (*current)->type != TOKEN_WORD)
		return;
	if (pipe(pipe_fd) == -1)
	{
		perror("pipe failed!");
		return;
	}
	pid = fork();
	if (pid == 0)
		child_process_heredoc(pipe_fd, current, heredoc_input, env);
	else if (pid > 0)
	{
		parent_process_heredoc(pipe_fd,heredoc_input,env,args);
	}
	else
	{
		perror("fork failed!");
		return;
	}
	*current = (*current)->next;
}



void	openfile_redirected(t_token **current, int *out_fd, char **args,
		int append)
{
	int	flag;
	int new_fd;

	flag = O_WRONLY | O_CREAT;
	if (append)
		flag |= O_APPEND;
	else
		flag |= O_TRUNC;
	if (*current && (*current)->type == TOKEN_WORD)
	{
		new_fd= open((*current)->value, flag, 0644);
		if (new_fd < 0)
		{
			write(2, "minishell: ", 11);
			write(2, (*current)->value,ft_strlen((*current)->value));
			write(2,": No such file or directory\n", 29);
			*args = NULL;
			return ;
		}
		if(*out_fd != STDOUT_FILENO)
			close(*out_fd);
		*out_fd = new_fd;
		*current = (*current)->next;
	}
}
