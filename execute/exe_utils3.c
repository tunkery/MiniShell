/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exe_utils3.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hpehliva <hpehliva@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/28 23:07:43 by hpehliva          #+#    #+#             */
/*   Updated: 2025/04/28 23:08:26 by hpehliva         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static int	count_token_args(t_token *tmp, int total)
{
	t_token	*tokens;
	t_token	*next;

	tokens = tmp;
	next = NULL;
	while (tokens && tokens->type != TOKEN_SEMIC && tokens->type != TOKEN_PIPE)
	{
		if (tokens && tokens == next)
		{
			next = NULL;
			tokens = tokens->next;
			continue ;
		}
		if (count_token_helper(tokens) == -1)
		{
			next = tokens->next;
			tokens = tokens->next;
			continue ;
		}
		if (tokens->type == TOKEN_WORD)
			total++;
		tokens = tokens->next;
	}
	return (total);
}

char	**tokens_to_args(t_token *tokens, t_env *env)
{
	t_token	*tmp;
	char	**args;
	int		i;
	int		total;

	total = 0;
	total = count_token_args(tokens, 0);
	args = my_malloc(env->s_gc, (total + 1) * sizeof(char *));
	if (!args)
		return (NULL);
	tmp = tokens;
	i = 0;
	while (tmp && tmp->type != TOKEN_SEMIC && tmp->type != TOKEN_PIPE)
	{
		if (token_to_args_helper(&tmp) == -1)
			continue ;
		if (tmp->type == TOKEN_WORD)
			token_to_args_helper1(&args, tmp, env, &i);
		tmp = tmp->next;
	}
	args[i] = NULL;
	return (args);
}

void	execute_with_redirection(char **args, t_env *env, int out_fd,
		int save_stdout)
{
	if (builtin_check(args) != 0)
	{
		if (out_fd != STDOUT_FILENO)
		{
			if (dup2(out_fd, STDOUT_FILENO) == -1)
				perror("dup2 failed!");
			close(out_fd);
		}
		run_builtin(args, env);
		if (out_fd != STDOUT_FILENO)
		{
			if (dup2(save_stdout, STDOUT_FILENO) == -1)
				perror("dup2 failed!");
		}
	}
	else
	{
		exec_command(args, env, out_fd);
	}
}

t_token	*find_last_heredoc(t_token *start, t_token *end)
{
	t_token	*temp;
	t_token	*last_heredoc;

	temp = start;
	last_heredoc = NULL;
	while (temp && temp != end && temp->type != TOKEN_PIPE
		&& temp->type != TOKEN_SEMIC)
	{
		if (temp->type == TOKEN_HEREDOC)
			last_heredoc = temp;
		temp = temp->next;
	}
	return (last_heredoc);
}

t_token	*find_end_command(t_token *start)
{
	t_token	*end;

	end = start;
	while (end && end->type != TOKEN_PIPE && end->type != TOKEN_SEMIC)
		end = end->next;
	return (end);
}
