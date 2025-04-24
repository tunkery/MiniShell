/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_utils2.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bolcay <bolcay@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/24 15:39:02 by bolcay            #+#    #+#             */
/*   Updated: 2025/04/24 15:59:25 by bolcay           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	pid_is_zero(int *pipe, char **heredoc_input, t_env *env, t_token *curr)
{
	close(pipe[0]);
	set_signal_heredoc();
	*heredoc_input = handler_heredoc(curr->next->value, env);
	write(pipe[1], *heredoc_input, ft_strlen(*heredoc_input));
	close(pipe[1]);
	exit(0);
}

int	pipe_helper(int *pip, char **heredoc_input, t_env *env, t_token *curr)
{
	pid_t	pid;

	if (pipe(pip) == -1)
	{
		perror("pipe failed");
		return (0);
	}
	pid = fork();
	if (pid == -1)
	{
		perror("fork failed");
		close(pip[0]);
		close(pip[1]);
		return (0);
	}
	if (pid == 0)
		pid_is_zero(pip, heredoc_input, env, curr);
	else
		if (!parent_process_heredoc_pipe(pip, pid, curr, env))
			return (0);
	return (0);
}

void	heredoc_helper(char **heredoc, t_token **curr, int *pipe_fd, t_env *env)
{
	*heredoc = handler_heredoc((*curr)->value, env);
	write(pipe_fd[1], *heredoc, ft_strlen(*heredoc));
	close(pipe_fd[1]);
}
