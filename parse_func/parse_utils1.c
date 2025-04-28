/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_utils1.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hpehliva <hpehliva@student.42heilbronn.de  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/28 13:10:40 by hpehliva          #+#    #+#             */
/*   Updated: 2025/04/28 13:10:42 by hpehliva         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	set_heredoc_pipe(int pipe_fd[2])
{
	if (pipe(pipe_fd) == -1)
	{
		perror("pipe failed");
		return (0);
	}
	return (1);
}

int	create_heredoc_child(int pipe_fd[2], pid_t *pid)
{
	*pid = fork();
	if (*pid == -1)
	{
		perror("fork failed");
		close(pipe_fd[0]);
		close(pipe_fd[1]);
		return (0);
	}
	return (1);
}

void	execute_child_heredoc(int pipe_fd[2], t_token *curr, t_env *env)
{
	char	*heredoc_input;

	heredoc_input = NULL;
	close(pipe_fd[0]);
	set_signal_heredoc();
	heredoc_input = handler_heredoc(curr->next->value, env, 0);
	write(pipe_fd[1], heredoc_input, ft_strlen(heredoc_input));
	close(pipe_fd[1]);
	exit(0);
}

int	process_heredoc_token(t_token *curr, t_env *env)
{
	int		pipe_fd[2];
	pid_t	pid;

	if (!set_heredoc_pipe(pipe_fd))
		return (0);
	if (!create_heredoc_child(pipe_fd, &pid))
		return (0);
	if (pid == 0)
		execute_child_heredoc(pipe_fd, curr, env);
	else
		return (parent_process_heredoc_pipe(pipe_fd, pid, curr, env));
	return (1);
}

t_token	*find_segment_end(t_token **seg, int i, int seg_count)
{
	if (i < seg_count - 1)
		return (seg[i + 1]->prev);
	else
		return (NULL);
}
