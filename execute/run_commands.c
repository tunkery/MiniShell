/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   run_commands.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bolcay <bolcay@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/25 13:26:21 by bolcay            #+#    #+#             */
/*   Updated: 2025/04/29 21:49:29 by bolcay           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static void	run_without_path(char **args, t_env *env, int out_fd, char *exe)
{
	pid_t	pid;

	pid = fork();
	if (pid == 0)
	{
		if (out_fd != STDOUT_FILENO)
		{
			dup2(out_fd, STDOUT_FILENO);
			close(out_fd);
		}
		if (execve(exe, args, env->envp) == -1)
		{
			perror("execve failed");
			exit(127);
		}
	}
	else if (pid < 0)
		perror("fork Failed");
	else
		wait_for_child(pid, env);
}

static int	permission_check(char *str, t_env *env)
{
	struct stat	info;

	if (stat(str, &info) != 0)
	{
		no_file(str);
		env->exit_code = 127;
		return (1);
	}
	else if (S_ISDIR(info.st_mode))
	{
		write(2, "minishell: ", 11);
		write(2, str, ft_strlen(str));
		write(2, ": is a directory\n", 17);
		env->exit_code = 126;
		return (1);
	}
	else if (access(str, X_OK) != 0)
	{
		write(2, "minishell: ", 11);
		write(2, str, ft_strlen(str));
		write(2, ": Permission denied\n", 20);
		env->exit_code = 126;
		return (1);
	}
	return (0);
}

static void	run_with_path(char *str, char **args, t_env *env, int out_fd)
{
	int	check;

	check = permission_check(str, env);
	if (access(str, X_OK) == 0 && check == 0)
	{
		run_with_path_helper(str, args, env, out_fd);
	}
	else if (check == 0)
	{
		write(2, "minishell: ", 11);
		write(2, str, ft_strlen(str));
		write(2, ": command not found.\n", 21);
		env->exit_code = 127;
	}
}

static void	run_commands_helper(char **args, t_env *env, int out_fd
		, char *exec_path)
{
	if (!exec_path)
	{
		env->exit_code = 127;
		write(2, "minishell: ", 11);
		write(2, args[0], ft_strlen(args[0]));
		write(2, ": command not found.\n", 21);
	}
	else
		run_without_path(args, env, out_fd, exec_path);
}

void	exec_command(char **args, t_env *env, int out_fd)
{
	char	*exec_path;
	char	*path;
	char	*final;
	int		is_cat_command;

	if (!args || !args[0])
		return ;
	is_cat_command = 0;
	if (args[0] && ft_strcmp(args[0], "cat") == 0
		&& (!args[1] || args[1][0] == '-' ))
	{
		is_cat_command = 1;
		set_signal_backslash();
	}
	final = args[0];
	path = find_path(env);
	exec_path = find_exec(final, path, 0, env);
	gc_register(env->s_gc, exec_path);
	if (ft_strchr(final, '/'))
		run_with_path(final, args, env, out_fd);
	else
		run_commands_helper(args, env, out_fd, exec_path);
	if (is_cat_command)
		set_for_cat();
}
