/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exe_with_token.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: batuhan <batuhan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/25 14:24:26 by hpehliva          #+#    #+#             */
/*   Updated: 2025/04/15 17:07:05 by batuhan          ###   ########.fr       */
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
			perror(CYAN"Open failed!"RESET);
			clean_2d(args);
			*args = NULL;
			env->exit_code = 1;
			return;
		}
		*current = (*current)->next;
	}
	if(*in_fd  != STDIN_FILENO)
	{
		if(dup2(*in_fd, STDIN_FILENO) == -1)
		{
			close(*in_fd);
			perror("dub2 failed for redirected!");
			*args = NULL;
			clean_2d(args);
			env->exit_code = 1;
			return ;
		}
		close(*in_fd);
	}
}

static void	child_process_heredoc(int *pipe_fd, t_token **current,
		char **heredoc_input, t_env *env)
{
	close(pipe_fd[0]);
	// close the signal
	signal(SIGINT,SIG_DFL);

	*heredoc_input = handler_heredoc((*current)->value, env);
	write(pipe_fd[1], *heredoc_input, ft_strlen(*heredoc_input));
	close(pipe_fd[1]);
	free(*heredoc_input);
	exit(0);
}

static void	parent_process_heredoc(int *pipe_fd, char **args,pid_t pid)
{
	close(pipe_fd[1]);
	if (dup2(pipe_fd[0], STDIN_FILENO) == -1)
	{
		perror("dub2 failed!\n");
		clean_2d(args);
		close(pipe_fd[0]);
		return ;
	}
	close(pipe_fd[0]);
	// wait(NULL); // Alt sureci bekle
	int status;
	waitpid(pid, &status, 0);
	if(WIFSIGNALED(status)) // IF child was terminated by a signal!
	{
		clean_2d(args);
		*args = NULL;
		return;
	}

}

void	process_child_heredoc(t_token **current, char **heredoc_input,
		char **args, t_env *env)
{
		int pipe_fd[2];
		pid_t pid;

	if (*current && (*current)->type == TOKEN_WORD)
	{
		if (pipe(pipe_fd) == -1)
		{
			perror("pipe failed!");
			clean_2d(args);
			return ;
		} // Surecler arasi boruyu olusturur > alt surec icin
		pid = fork();
		if (pid == 0)
			child_process_heredoc(pipe_fd, current, heredoc_input, env);
		else if (pid > 0)
			parent_process_heredoc(pipe_fd, args,pid);
		else
		{
			perror("fork failed!");
			clean_2d(args);
			return ;
		}
		*current = (*current)->next;
	}
}

void	openfile_redirected(t_token **current, int *out_fd, char **args,
		int append)
{
	int	flag;

	flag = O_WRONLY | O_CREAT;
	if (append)
		flag |= O_APPEND;
	else
		flag |= O_TRUNC;
	if (*current && (*current)->type == TOKEN_WORD)
	{
		*out_fd = open((*current)->value, flag, 0644); // Uzerine yazdiriyor.
		if (*out_fd < 0)
		{
			perror("open failed!");
			clean_2d(args);
			return ;
		}
	}
}
