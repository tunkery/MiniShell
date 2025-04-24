/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   run_commands.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bolcay <bolcay@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/25 13:26:21 by bolcay            #+#    #+#             */
/*   Updated: 2025/04/24 18:40:02 by bolcay           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	wait_for_child(pid_t pid, t_env *env)
{
	int	status;

	waitpid(pid, &status, 0);
	if (WIFEXITED(status))
		env->exit_code = WEXITSTATUS(status);
	else if (WIFSIGNALED(status))
		env->exit_code = 128 + WTERMSIG(status);
}

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
			// perror("execvp Failed");
			// if (exe)
			// 	free(exe);
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
		fprintf(stderr, "minishell: %s: No such file or directory\n", str);
		env->exit_code = 127;
		return (1);
	}
	else if (S_ISDIR(info.st_mode))
	{
		fprintf(stderr, "minishell: %s: is a directory\n", str);
		env->exit_code = 126;
		return (1);
	}
	else if (access(str, X_OK) != 0)
	{
		fprintf(stderr, "minishell: %s: Permission denied\n", str);
		env->exit_code = 126;
		return (1);
	}
	return (0);
}

static void	run_with_path(char *str, char **args, t_env *env, int out_fd)
{
	int		check;

	check = permission_check(str, env);
	if (access(str, X_OK) == 0 && check == 0)
	{
		run_with_path_helper(str, args, env, out_fd);
	}
	else if (check == 0)
	{
		fprintf(stderr, "minishell: %s: command not found.\n", str);
		env->exit_code = 127;
	}
}

void	exec_command(char **args, t_env *env, int out_fd)
{
	char	*exec_path;
	char	*path;
	char	*final;

	if (!args || !args[0])
		return ;
	final = args[0];
	path = find_path(env);
	exec_path = find_exec(final, path, 0, env);
	gc_register(env->s_gc, exec_path);
	if (ft_strchr(final, '/'))
		run_with_path(final, args, env, out_fd);
	else
	{
		if (!exec_path)
		{
			env->exit_code = 127;
			fprintf(stderr, "minishell: %s: command not found.\n", args[0]);
		}
		else
			run_without_path(args, env, out_fd, exec_path);
	}
}
