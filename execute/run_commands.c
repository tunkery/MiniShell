/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   run_commands.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bolcay <bolcay@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/25 13:26:21 by bolcay            #+#    #+#             */
/*   Updated: 2025/03/25 13:51:35 by bolcay           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static void	wait_for_child(pid_t pid, t_env *env)
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
		if (execve(exe, args, env->path) == -1)
		{
			perror("execvp Failed");
			free(exe);
			exit(127);
		}
	}
	else if (pid < 0)
		perror("fork Failed");
	else
		wait_for_child(pid, env);
}

static void	run_with_path(char **args, t_env *env, int out_fd)
{
	pid_t	pid;

	if (access(args[0], X_OK) == 0)
	{
		pid = fork();
		if (pid == 0)
		{
			if (out_fd != STDOUT_FILENO)
			{
				dup2(out_fd, STDOUT_FILENO);
				close(out_fd);
			}
			if (execve(args[0], args, env->path) == -1)
			{
				perror("execvp Failed");
				exit(127);
			}
		}
		else if (pid < 0)
			perror("fork Failed");
		else
			wait_for_child(pid, env);
	}
	else
		printf("minishell: %s: command not found.\n", args[0]);
}

void	exec_command(char **args, t_env *env, int out_fd)
{
	char	*exec_path;

	if (!args || !args[0])
		return ;
	if (ft_strchr(args[0], '/')) // if, yani eger, biri path ile yazmissa komutu burasi kontrol edip calistiriyo
	{
		exec_path = NULL;
		run_with_path(args, env, out_fd);
	}
	else
		// eger pathsiz yazilmissa command burasi isi eline aliyo ve pathi bulup calistiriyo
	{
		exec_path = find_exec(args[0], env->path1, 0, 5);
		if (!exec_path)
		{
			printf("minishell: %s: command not found.\n", args[0]);
			return ;
		}
		run_without_path(args, env, out_fd, exec_path);
	}
	DEBUG_PRINT(GRN "PID exit status: %d\n" RESET, env->exit_code);
	if (exec_path)
		free(exec_path);
}
