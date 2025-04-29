/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exe_with_token2.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: batuhan <batuhan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/28 23:11:27 by hpehliva          #+#    #+#             */
/*   Updated: 2025/04/29 19:57:54 by batuhan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	get_heredoc_delimiter(t_token **current, char **delimiter,
		int *quote_mode, t_env *env)
{
	size_t	len;
	char	*unquoted;

	*delimiter = (*current)->value;
	*quote_mode = 0;
	len = ft_strlen(*delimiter);
	if (len >= 2 && (((*delimiter)[0] == '\'' && (*delimiter)[len - 1] == '\'')
			|| (len >= 2 && (*delimiter)[0] == '\"' && (*delimiter)[len
				- 1] == '\"')))
	{
		*quote_mode = 1;
		unquoted = ft_substr(*delimiter, 1, len - 2);
		gc_register(env->s_gc, unquoted);
		*delimiter = unquoted;
	}
	else if ((*current)->prev && (*current)->prev->value
		&& ((ft_strncmp((*current)->prev->value, "<<'", 3) == 0)
			|| (ft_strncmp((*current)->prev->value, "<<\"", 3) == 0)))
		*quote_mode = 1;
}

void	child_process_heredoc(int *pipe_fd, t_token **current,
		char **heredoc_input, t_env *env)
{
	char	*delimiter;
	int		quote_mode;
	char	*content;

	delimiter = NULL;
	quote_mode = 0;
	content = NULL;
	close(pipe_fd[0]);
	get_heredoc_delimiter(current, &delimiter, &quote_mode, env);
	content = handler_heredoc(delimiter, env, quote_mode);
	gc_register(env->s_gc, content);
	*heredoc_input = content;
	write(pipe_fd[1], *heredoc_input, ft_strlen(*heredoc_input));
	close(pipe_fd[1]);
	exit(0);
}

void	process_heredoc_result(int fd, char **heredoc_input, t_env *env,
		int stat)
{
	char	fd_str[16];

	if (WIFSIGNALED(stat))
	{
		close(fd);
		return ;
	}
	snprintf(fd_str, sizeof(fd_str), "%d", fd);
	if (*heredoc_input)
		free(*heredoc_input);
	*heredoc_input = ft_strdup(fd_str);
	gc_register(env->s_gc, *heredoc_input);
}

void	parent_process_heredoc(int pipe_fd[2], char **heredoc_input, t_env *env,
		char **args)
{
	int	status;

	close(pipe_fd[1]);
	waitpid(-1, &status, 0);
	if (WIFSIGNALED(status))
	{
		close(pipe_fd[0]);
		*args = NULL;
		env->exit_code = 0;
		return ;
	}
	process_heredoc_result(pipe_fd[0], heredoc_input, env, status);
}
