/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: batuhan <batuhan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/13 16:02:56 by hpehliva          #+#    #+#             */
/*   Updated: 2025/04/22 16:41:37 by batuhan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static void	redirect_output(t_token **curr, int *fd, int flags)
{
	*curr = (*curr)->next;
	if (*curr && (*curr)->type == TOKEN_WORD)
	{
		if (*fd != STDOUT_FILENO)
			close(*fd);
		*fd = open((*curr)->value, flags, 0644);
		if ((*fd < 0))
		{
			perror("open failed");
			exit(1);
		}
	}
}

static void	redirect_input(t_token **curr, int *fd)
{
	*curr = (*curr)->next;
	if (*curr && (*curr)->type == TOKEN_WORD)
	{
		if (*fd != STDIN_FILENO)
			close(*fd);
		*fd = open((*curr)->value, O_RDONLY);
		if (*fd < 0)
		{
			perror("open failed");
			exit(1);
		}
	}
}

void	handle_standard_redirec(t_token **curr, int *in_fd, int *out_fd)
{
	if ((*curr)->type == TOKEN_REDIRECT_OUT)
		redirect_output(curr, out_fd, O_WRONLY | O_CREAT | O_TRUNC);
	else if ((*curr)->type == TOKEN_REDIRECT_APPEND)
		redirect_output(curr, out_fd, O_WRONLY | O_CREAT | O_APPEND);
	else if ((*curr)->type == TOKEN_REDIRECT_IN)
		redirect_input(curr, in_fd);
}

void	handle_heredoc_redirec(t_token **curr, int *in_fd, t_env *env)
{
	*curr = (*curr)->next;
	if (*curr && (*curr)->type == TOKEN_WORD)
	{
		if ((*curr)->prev && (*curr)->prev->type == TOKEN_WORD)
		{
			if (*in_fd != STDIN_FILENO)
				close(*in_fd);
			*in_fd = ft_atoi((*curr)->prev->value);
		}
		else
			setup_pipe_heredoc(*curr, in_fd, env);
	}
}

void	apply_redirections(t_token *start, t_token *end, int *fds, t_env *env)
{
	t_token	*current;
	t_token	*prev;
	int		*in_fd;
	int		*out_fd;

	in_fd = &fds[0];
	out_fd = &fds[1];
	current = start;
	while (current && current != end && current->type != TOKEN_PIPE)
	{
		prev = current;
		process_apply_redirections(&current, in_fd, out_fd, env);
		if (current == prev)
			current = current->next;
	}
}
