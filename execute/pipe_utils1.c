/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_utils1.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hpehliva <hpehliva@student.42heilbronn.de  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/28 17:28:39 by hpehliva          #+#    #+#             */
/*   Updated: 2025/04/28 17:28:40 by hpehliva         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	init_pipe_seg(t_pipe_seg *stat, int **pipes, int seg_count, int i)
{
	stat->pipes = pipes;
	stat->seg_count = seg_count;
	stat->seg_index = i;
	stat->fds[0] = STDIN_FILENO;
	stat->fds[1] = STDOUT_FILENO;
}

void	find_seg_redirect(int *fds, t_token *start, t_token *end, t_env *env)
{
	int	redirec_fds[2];

	redirec_fds[0] = fds[0];
	redirec_fds[1] = fds[1];
	apply_redirections(start, end, redirec_fds, env);
	if (redirec_fds[0] != fds[0])
	{
		if (fds[0] != STDIN_FILENO)
			close(fds[0]);
		fds[0] = redirec_fds[0];
	}
	if (redirec_fds[1] != fds[1])
	{
		if (fds[1] != STDOUT_FILENO)
			close(fds[1]);
		fds[1] = redirec_fds[1];
	}
}

void	setup_child_pipes(t_pipe_seg *stat, int *fds)
{
	int	i;
	int	seg_count;
	int	j;

	i = stat->seg_index;
	seg_count = stat->seg_count;
	fds[0] = STDIN_FILENO;
	fds[1] = STDOUT_FILENO;
	j = 0;
	if (i > 0)
	{
		fds[0] = stat->pipes[i - 1][0];
	}
	if (i < seg_count - 1)
	{
		fds[1] = stat->pipes[i][1];
	}
	while (j < seg_count - 1)
	{
		if (j != i - 1)
			close(stat->pipes[j][0]);
		if (j != i)
			close(stat->pipes[j][1]);
		j++;
	}
}

void	exec_cmd_or_buildin(t_token *seg_start, t_token *seg_end, t_env *env)
{
	char	**args;

	args = create_args_from_tokens(seg_start, seg_end, env);
	if (!args || !args[0])
		exit(EXIT_FAILURE);
	if (builtin_check(args))
	{
		run_builtin(args, env);
		exit(env->exit_code);
	}
	else
	{
		exec_command(args, env, STDOUT_FILENO);
		exit(env->exit_code);
	}
}

void	exec_child_comd(t_pipe_seg *stat, t_env *env)
{
	setup_child_pipes(stat, stat->fds);
	find_seg_redirect(stat->fds, stat->seg_start, stat->seg_end, env);
	if (!setup_io(stat->fds[0], stat->fds[1]))
		exit(EXIT_FAILURE);
	exec_cmd_or_buildin(stat->seg_start, stat->seg_end, env);
}
