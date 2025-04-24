/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bolcay <bolcay@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/13 16:02:31 by hpehliva          #+#    #+#             */
/*   Updated: 2025/04/24 16:31:32 by bolcay           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	find_seg_redirect(int *in_fd, int *out_fd, t_token *start, t_token *end,
		t_env *env)
{
	int	redirec_in;
	int	redirec_out;

	redirec_in = *in_fd;
	redirec_out = *out_fd;
	apply_redirections(start, end, &redirec_in, &redirec_out, env);
	if (redirec_in != *in_fd)
	{
		if (*in_fd != STDIN_FILENO)
			close(*in_fd);
		*in_fd = redirec_in;
	}
	if (redirec_out != *out_fd)
	{
		if (*out_fd != STDOUT_FILENO)
			close(*out_fd);
		*out_fd = redirec_out;
	}
}

void	setup_child_pipes(int **pipes, int i, int seg_count, int *in_fd,
		int *out_fd)
{
	int	j;

	*in_fd = STDIN_FILENO;
	*out_fd = STDOUT_FILENO;
	j = 0;
	if (i > 0)
		*in_fd = pipes[i - 1][0];
	if (i < seg_count - 1)
		*out_fd = pipes[i][1];
	while (j < seg_count - 1)
	{
		if (j != i - 1)
			close(pipes[j][0]);
		if (j != i)
			close(pipes[j][1]);
		j++;
	}
}

void	exec_child_comd(t_token *seg_start, t_token *seg_end, t_env *env,
		int **pipes, int i, int seg_count)
{
	int		in_fd;
	int		out_fd;
	char	**args;

	setup_child_pipes(pipes, i, seg_count, &in_fd, &out_fd);
	find_seg_redirect(&in_fd, &out_fd, seg_start, seg_end, env);
	if (!setup_io(in_fd, out_fd))
		exit(EXIT_FAILURE);
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

int	fork_cmd_process(t_token **segments, int seg_count, t_env *env, int **pipes,
		pid_t *pids)
{
	int		i;
	t_token	*seg_end;

	i = 0;
	while (i < seg_count)
	{
		pids[i] = fork();
		if (pids[i] < 0)
		{
			perror("fork failed");
			return (0);
		}
		if (pids[i] == 0)
		{
			if (i < seg_count - 1)
				seg_end = segments[i + 1]->prev;
			else
				seg_end = NULL;
			exec_child_comd(segments[i], seg_end, env, pipes, i, seg_count);
			exit(EXIT_FAILURE);
		}
		i++;
	}
	return (1);
}

void	execute_piped_command(t_token *tokens, t_env *env)
{
	int		seg_count;
	t_token	**segments;
	int		**pipes;
	pid_t	*pids;

	seg_count = 0;
	segments = find_pipe_seg(tokens, &seg_count, env);
	if (!segments || seg_count <= 0)
		return ;
	set_signal_pipe();
	if (!preprocess_heredocs(segments, seg_count, env))
		return ;
	pipes = create_pipes(seg_count);
	if (!pipes && seg_count > 1)
		return ;
	pids = my_malloc(env->s_gc, sizeof(pid_t) * seg_count);
	if (!pids)
	{
		if (pipes)
			cleanup_pipes(pipes, seg_count);
		return ;
	}
	if (!fork_cmd_process(segments, seg_count, env, pipes, pids))
	{
		if (pipes)
			cleanup_pipes(pipes, seg_count);
		return ;
	}
	if (pipes)
		cleanup_pipes(pipes, seg_count);
	wait_child_pipes(pids, seg_count, env);
}
