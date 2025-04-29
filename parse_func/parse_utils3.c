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

	close(pipe_fd[1]);
	waitpid(pid, &status, 0);
	if (WIFSIGNALED(status))
	{
		close(pipe_fd[0]);
		env->exit_code = 128 + WTERMSIG(status);
		return (0);
	}
	curr->value = ft_itoa(pipe_fd[0]);
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

void	process_apply_redirections(t_token **current, int *in_fd, int *out_fd,
		t_env *env)
{
	if ((*current)->type == TOKEN_HEREDOC_PROCESSED)
	{
		if (*in_fd != STDIN_FILENO)
			close(*in_fd);
		*in_fd = ft_atoi((*current)->value);
		*current = (*current)->next->next;
		return ;
	}
	else if ((*current)->type == TOKEN_HEREDOC)
		handle_heredoc_redirec(current, in_fd, env);
	else if ((*current)->type == TOKEN_REDIRECT_OUT
		|| (*current)->type == TOKEN_REDIRECT_APPEND
		|| (*current)->type == TOKEN_REDIRECT_IN)
		handle_standard_redirec(current, in_fd, out_fd);
	else
	{
		if (*current)
			*current = (*current)->next;
	}
}
