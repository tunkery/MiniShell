/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bolcay <bolcay@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/13 16:02:56 by hpehliva          #+#    #+#             */
/*   Updated: 2025/04/24 21:26:05 by bolcay           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	**create_args_from_tokens(t_token *start, t_token *end, t_env *env)
{
	int	count;

	if (!start)
		return (NULL);
	count = count_args_seg(start, end);
	return (args_from_token_alloc(start, end, count, env));
}

static void	redirect_output(t_token **curr, int *fd, int flags)
{
	*curr = (*curr)->next;
	if (*curr && (*curr)->type == TOKEN_WORD)
	{
		if (*fd != STDOUT_FILENO)
			close(*fd);
		*fd = open ((*curr)->value, flags, 0644);
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
		redirect_output(curr, out_fd, O_WRONLY | O_CREAT | O_APPEND);
	else if ((*curr)->type == TOKEN_REDIRECT_APPEND)
		redirect_output(curr, out_fd, O_WRONLY | O_CREAT | O_APPEND);
	else if ((*curr)->type == TOKEN_REDIRECT_IN)
		redirect_input(curr, in_fd);
}

// void	handle_standard_redirec(t_token **curr, int *in_fd, int *out_fd)
// {
// 	if ((*curr)->type == TOKEN_REDIRECT_OUT)
// 	{
// 		*curr = (*curr)->next;
// 		if (*curr && (*curr)->type == TOKEN_WORD)
// 		{
// 			if (*out_fd != STDOUT_FILENO)
// 				close(*out_fd);
// 			*out_fd = open((*curr)->value, O_WRONLY | O_CREAT | O_TRUNC, 0644);
// 			if (*out_fd < 0)
// 			{
// 				perror("open failed");
// 				exit(1);
// 			}
// 		}
// 	}
// 	else if ((*curr)->type == TOKEN_REDIRECT_APPEND)
// 	{
// 		*curr = (*curr)->next;
// 		if (*curr && (*curr)->type == TOKEN_WORD)
// 		{
// 			if (*out_fd != STDOUT_FILENO)
// 				close(*out_fd);
// 			*out_fd = open((*curr)->value, O_WRONLY | O_CREAT | O_APPEND, 0644);
// 			if (*out_fd < 0)
// 			{
// 				perror("open failed");
// 				exit(1);
// 			}
// 		}
// 	}
// 	else if ((*curr)->type == TOKEN_REDIRECT_IN)
// 	{
// 		*curr = (*curr)->next;
// 		if (*curr && (*curr)->type == TOKEN_WORD)
// 		{
// 			if (*in_fd != STDIN_FILENO)
// 				close(*in_fd);
// 			*in_fd = open((*curr)->value, O_RDONLY);
// 			if (*in_fd < 0)
// 			{
// 				perror("open failed");
// 				exit(1);
// 			}
// 		}
// 	}
// }

void	handle_heredoc_redirec(t_token **curr, int *in_fd, t_env *env)
{
	char	*heredoc_input;
	int		pipe_fd[2];

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
		{
			if (*in_fd != STDIN_FILENO)
				close(*in_fd);
			if (pipe(pipe_fd) == -1)
			{
				perror("pipe failed");
				exit(EXIT_FAILURE);
			}
			heredoc_helper(&heredoc_input, curr, pipe_fd, env);
			*in_fd = pipe_fd[0];
		}
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
