/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bolcay <bolcay@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 13:33:50 by hpehliva          #+#    #+#             */
/*   Updated: 2025/04/24 15:54:52 by bolcay           ###   ########.fr       */
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

int	preprocess_heredocs(t_token **seg, int seg_count, t_env *env)
{
	int		i;
	t_token	*curr;
	t_token	*seg_end;
	char	*heredoc_input;
	int		pipe_fd[2];

	i = 0;
	heredoc_input = NULL;
	while (i < seg_count)
	{
		curr = seg[i];
		if (i < seg_count - 1)
			seg_end = seg[i + 1]->prev;
		else
			seg_end = NULL;
		while (curr && curr != seg_end && curr->type != TOKEN_PIPE)
		{
			if (curr->type == TOKEN_HEREDOC && curr->next
				&& curr->next->type == TOKEN_WORD)
				return (pipe_helper(pipe_fd, &heredoc_input, env, curr));
			curr = curr->next;
		}
		i++;
	}
	return (1);
}

// Helper function to check if tokens contain pipe
int	count_pipe_seg(t_token *tokens)
{
	t_token	*tmp;
	int		count;

	tmp = tokens;
	count = 0;
	while (tmp && tmp->type != TOKEN_SEMIC)
	{
		if (tmp->type == TOKEN_PIPE)
			count++;
		tmp = tmp->next;
	}
	count++;
	return (count);
}

t_token	**seg_alloc(t_token *tokens, int seg_count, t_env *env)
{
	t_token	**segments;
	t_token	*tmp;
	int		i;

	segments = my_malloc(env->s_gc, sizeof(t_token *) * (seg_count + 1));
	if (!segments)
		return (NULL);
	tmp = tokens;
	i = 0;
	segments[i++] = tmp;
	while (tmp && tmp->type != TOKEN_SEMIC)
	{
		if (tmp->type == TOKEN_PIPE)
		{
			segments[i++] = tmp->next;
		}
		tmp = tmp->next;
	}
	segments[i] = NULL;
	return (segments);
}

char	**args_from_token_alloc(t_token *start, t_token *end, int count,
		t_env *env)
{
	char	**args;
	int		i;
	t_token	*tmp;

	args = my_malloc(env->s_gc, sizeof(char *) * (count + 1));
	if (!args)
		return (NULL);
	i = 0;
	tmp = start;
	while (tmp && tmp != end && tmp->type != TOKEN_PIPE)
	{
		if (token_alloc_check(tmp) == -1)
		{
			tmp = tmp->next;
			continue ;
		}
		if (token_alloc_check1(tmp) == -1)
		{
			args[i] = ft_strdup(tmp->value);
			gc_register(env->s_gc, args[i++]);
		}
		tmp = tmp->next;
	}
	args[i] = NULL;
	return (args);
}
