/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exe_utils1.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bolcay <bolcay@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/24 18:08:37 by bolcay            #+#    #+#             */
/*   Updated: 2025/04/24 20:19:38 by bolcay           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	run_with_path_helper(char *str, char **args, t_env *env, int out_fd)
{
	pid_t	pid;

	pid = fork();
	if (pid == 0)
	{
		if (out_fd != STDOUT_FILENO)
		{
			dup2(out_fd, STDOUT_FILENO);
			close(out_fd);
		}
		if (execve(str, args, env->envp) == -1)
		{
			perror("execve Failed");
			exit(127);
		}
	}
	else if (pid < 0)
		perror("fork Failed");
	else
		wait_for_child(pid, env);
}

void	create_pipe_helper(int ***pipes, int i)
{
	int	j;

	j = 0;
	while (j < i)
	{
		close(*pipes[j][0]);
		close(*pipes[j][1]);
		free(*pipes[j]);
		j++;
	}
	free(*pipes);
}

void	create_pipe_helper1(int ***pipes, int i)
{
	int	j;

	perror("pipe failed");
	j = 0;
	while (j < i)
	{
		close(*pipes[j][0]);
		close(*pipes[j][1]);
		free(*pipes[j]);
		j++;
	}
	free(*pipes[i]);
	free(*pipes);
}

void	exe_helper(t_token **temp)
{
	t_token	*tmp;

	tmp = *temp;
	while (tmp && tmp->type != TOKEN_SEMIC)
		tmp = tmp->next;
	if (tmp)
		tmp = tmp->next;
	*temp = tmp;
}

void	exe_helper1(int save_stdin)
{
	if (dup2(save_stdin, STDIN_FILENO) == -1)
		perror("dup2 failed to restore STDIN");
}
