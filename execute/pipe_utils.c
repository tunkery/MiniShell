/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bolcay <bolcay@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/13 16:02:43 by hpehliva          #+#    #+#             */
/*   Updated: 2025/04/24 20:13:54 by bolcay           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	has_pipes(t_token *tokens)
{
	t_token	*tmp;

	tmp = tokens;
	while (tmp && tmp->type != TOKEN_SEMIC)
	{
		if (tmp->type == TOKEN_PIPE)
			return (1);
		tmp = tmp->next;
	}
	return (0);
}

int	**create_pipes(int seg_count)
{
	int	i;
	int	j;
	int	**pipes;

	i = 0;
	j = 0;
	pipes = malloc(sizeof(int *) * (seg_count - 1));
	if (!pipes || seg_count <= 1)
		return (NULL);
	while (i < seg_count - 1)
	{
		pipes[i] = malloc(sizeof(int) * 2);
		if (!pipes[i])
		{
			create_pipe_helper(&pipes, i);
			return (NULL);
		}
		if (pipe(pipes[i]) == -1)
		{
			create_pipe_helper1(&pipes, i);
			return (NULL);
		}
		i++;
	}
	return (pipes);
}

void	wait_child_pipes(pid_t *pids, int seg_count, t_env *env)
{
	int	i;
	int	status;

	i = 0;
	while (i < seg_count)
	{
		waitpid(pids[i], &status, 0);
		if (i == seg_count - 1)
		{
			if (WIFEXITED(status))
				env->exit_code = WEXITSTATUS(status);
			else if (WIFSIGNALED(status))
				env->exit_code = 128 + WTERMSIG(status);
		}
		i++;
	}
}

void	cleanup_pipes(int **pipes, int seg_count)
{
	int	i;

	i = 0;
	while (i < seg_count - 1)
	{
		close(pipes[i][0]);
		close(pipes[i][1]);
		free(pipes[i]);
		i++;
	}
	free(pipes);
}

int	setup_io(int in_fd, int out_fd)
{
	if (in_fd != STDIN_FILENO)
	{
		if (dup2(in_fd, STDIN_FILENO) == -1)
		{
			perror("dup2 for stdin failed");
			return (0);
		}
		close(in_fd);
	}
	if (out_fd != STDOUT_FILENO)
	{
		if (dup2(out_fd, STDOUT_FILENO) == -1)
		{
			perror("dup2 for stdout failed");
			return (0);
		}
		close(out_fd);
	}
	return (1);
}
