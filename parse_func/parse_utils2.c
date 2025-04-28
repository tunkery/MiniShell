/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_utils2.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hpehliva <hpehliva@student.42heilbronn.de  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/28 13:12:38 by hpehliva          #+#    #+#             */
/*   Updated: 2025/04/28 13:12:40 by hpehliva         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	token_alloc_check1(t_token *tmp)
{
	if (tmp->type == TOKEN_WORD && !(tmp->prev
			&& (tmp->prev->type == TOKEN_REDIRECT_IN
				|| tmp->prev->type == TOKEN_REDIRECT_OUT
				|| tmp->prev->type == TOKEN_REDIRECT_APPEND
				|| tmp->prev->type == TOKEN_HEREDOC)))
	{
		return (-1);
	}
	return (0);
}

int	token_alloc_check(t_token *tmp)
{
	if (tmp->type == TOKEN_HEREDOC_PROCESSED || (tmp->prev
			&& tmp->prev->type == TOKEN_HEREDOC_PROCESSED) || (tmp->prev
			&& (tmp->prev->type == TOKEN_HEREDOC)))
	{
		return (-1);
	}
	return (0);
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

void	setup_pipe_heredoc(t_token *curr, int *in_fd, t_env *env)
{
	char	*heredoc_input;
	int		quote_mode;
	int		pipe_fd[2];

	heredoc_input = NULL;
	quote_mode = 0;
	if (*in_fd != STDIN_FILENO)
		close(*in_fd);
	if (pipe(pipe_fd) == -1)
	{
		perror("pipe failed");
		exit(EXIT_FAILURE);
	}
	heredoc_input = handler_heredoc(curr->value, env, quote_mode);
	write(pipe_fd[1], heredoc_input, ft_strlen(heredoc_input));
	close(pipe_fd[1]);
	*in_fd = pipe_fd[0];
}

char	**create_args_from_tokens(t_token *start, t_token *end, t_env *env)
{
	int	count;

	if (!start)
		return (NULL);
	count = count_args_seg(start, end);
	return (args_from_token_alloc(start, end, count, env));
}
