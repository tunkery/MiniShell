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

void	apply_redirections(t_token *start, t_token *end, int *in_fd,
		int *out_fd, t_env *env)
{
	t_token	*current;

	current = start;
	while (current && current != end && current->type != TOKEN_PIPE)
	{
		if (current->type == TOKEN_HEREDOC_PROCESSED)
		{
			if (*in_fd != STDIN_FILENO)
				close(*in_fd);
			*in_fd = ft_atoi(current->value);
			current = current->next->next;
			continue ;
		}
		else if (current->type == TOKEN_HEREDOC)
			handle_heredoc_redirec(&current, in_fd, env);
		else if (current->type == TOKEN_REDIRECT_OUT
			|| current->type == TOKEN_REDIRECT_APPEND
			|| current->type == TOKEN_REDIRECT_IN)
			handle_standard_redirec(&current, in_fd, out_fd);
		else
		{
			if (current)
				current = current->next;
		}
	}
}
