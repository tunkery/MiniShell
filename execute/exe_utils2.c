/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exe_utils2.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bolcay <bolcay@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/28 23:05:25 by hpehliva          #+#    #+#             */
/*   Updated: 2025/04/29 21:33:03 by bolcay           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	handle_out_process(t_token **current, int *out_fd, char **args,
		int append)
{
	*current = (*current)->next;
	openfile_redirected(current, out_fd, args, append);
}

void	handle_in_process(t_token **current, int *in_fd, char **args,
		t_env *env)
{
	*current = (*current)->next;
	read_redirected_in(current, in_fd, args, env);
}

void	handle_heredoc_process(t_token **current, char **args, t_env *env,
		t_token *last_heredoc)
{
	t_token	*curr_heredoc;
	char	*fd_str;
	int		fd;

	curr_heredoc = *current;
	fd_str = NULL;
	*current = (*current)->next;
	process_child_heredoc(current, &fd_str, args, env);
	if (!*args || !fd_str)
		return ;
	fd = ft_atoi(fd_str);
	if (curr_heredoc == last_heredoc)
	{
		dup2(fd, STDIN_FILENO);
	}
	close(fd);
}

void	setup_input_fd(int in_fd)
{
	if (in_fd != STDIN_FILENO)
	{
		if (dup2(in_fd, STDIN_FILENO) != -1)
		{
			perror("dup2 failed");
			close(in_fd);
			return ;
		}
		close(in_fd);
	}
}
