/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_utils3.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hpehliva <hpehliva@student.42heilbronn.de  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/28 13:15:21 by hpehliva          #+#    #+#             */
/*   Updated: 2025/04/28 13:15:22 by hpehliva         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	parent_process_heredoc_pipe(int *pipe_fd, pid_t pid, t_token *curr,
		t_env *env)
{
	int		status;
	char	fd_str[16];

	close(pipe_fd[1]);
	waitpid(pid, &status, 0);
	if (WIFSIGNALED(status))
	{
		close(pipe_fd[0]);
		return (0);
	}
	snprintf(fd_str, sizeof(fd_str), "%d", pipe_fd[0]);
	curr->value = ft_strdup(fd_str);
	gc_register(env->s_gc, curr->value);
	curr->type = TOKEN_HEREDOC_PROCESSED;
	return (1);
}

int	process_segment(t_token *curr, t_token *seg_end, t_env *env)
{
	while (curr && curr != seg_end && curr->type != TOKEN_PIPE)
	{
		if (curr->type == TOKEN_HEREDOC && curr->next
			&& curr->next->type == TOKEN_WORD)
		{
			if (!process_heredoc_token(curr, env))
				return (0);
		}
		curr = curr->next;
	}
	return (1);
}

int	preprocess_heredocs(t_token **seg, int seg_count, t_env *env)
{
	int		i;
	t_token	*curr;
	t_token	*seg_end;

	i = 0;
	while (i < seg_count)
	{
		curr = seg[i];
		seg_end = find_segment_end(seg, i, seg_count);
		if (!process_segment(curr, seg_end, env))
			return (0);
		i++;
	}
	return (1);
}
