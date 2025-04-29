/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exe_main.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hpehliva <hpehliva@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/24 12:04:28 by hpehliva          #+#    #+#             */
/*   Updated: 2025/04/28 23:08:36 by hpehliva         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	handle_redirection_helper(t_token **temp, char **args, int *out_fd,
		t_env *env)
{
	int		in_fd;
	t_token	*end;
	t_token	*last_heredoc;

	in_fd = STDIN_FILENO;
	end = find_end_command(*temp);
	last_heredoc = find_last_heredoc(*temp, end);
	while (*temp && *temp != end)
	{
		if ((*temp)->type == TOKEN_REDIRECT_OUT)
			handle_out_process(temp, out_fd, args, 0);
		else if ((*temp)->type == TOKEN_REDIRECT_IN)
			handle_in_process(temp, &in_fd, args, env);
		else if ((*temp)->type == TOKEN_REDIRECT_APPEND)
			handle_out_process(temp, out_fd, args, 1);
		else if ((*temp)->type == TOKEN_HEREDOC)
			handle_heredoc_process(temp, args, env, last_heredoc);
		else
			*temp = (*temp)->next;
		if (!*args)
			return ;
	}
	setup_input_fd(in_fd);
}

void	handle_redirection(t_token **current, char **args, int *out_fd,
		t_env *env)
{
	t_token	*temp;

	temp = *current;
	handle_redirection_helper(&temp, args, out_fd, env);
	*current = temp;
	if (!*args)
		return ;
}

void	exec_without_pipes(t_token *tokens, t_env *env, int out_fd)
{
	t_token	*tmp;
	char	**args;
	int		save_stdout;
	int		save_stdin;

	tmp = tokens;
	out_fd = STDOUT_FILENO;
	save_stdout = dup(STDOUT_FILENO);
	save_stdin = dup(STDIN_FILENO);
	while (tmp)
	{
		args = tokens_to_args(tmp, env);
		if (!args)
		{
			exe_helper(&tmp);
			continue ;
		}
		handle_redirection(&tmp, args, &out_fd, env);
		if (args && args[0] && args[0][0] != '\0')
			execute_with_redirection(args, env, out_fd, save_stdout);
		exe_helper1(save_stdin);
		exe_helper(&tmp);
	}
	close_both(save_stdout, save_stdin);
}

void	cell_launch(t_token *tokens, t_env *env)
{
	if (!validate_syntax(tokens, env))
	{
		return ;
	}
	if (has_pipes(tokens))
	{
		execute_piped_command(tokens, env);
	}
	else
	{
		exec_without_pipes(tokens, env, 0);
	}
}
