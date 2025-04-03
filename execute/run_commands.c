/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   run_commands.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bolcay <bolcay@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/25 13:26:21 by bolcay            #+#    #+#             */
/*   Updated: 2025/04/02 17:49:30 by bolcay           ###   ########.fr       */
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
		if (execve(exe, args, env->envp) == -1)
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
			if (execve(args[0], args, env->envp) == -1)
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

static char	*get_path(char *str, t_env *env)
{
	int		i;
	int		j;
	int		size;
	char	*key;
	char	*temp;
	char	*temp2;
	char	*final;

	i = 1;
	j = 0;
	key = ft_substr(str, 1, key_size(str));
	size = ft_strlen(key);
	while (env->envp[j] && ft_strncmp(env->envp[j], key, size) != 0)
		j++;
	if (!env->envp[j])
		return (NULL);
	temp = ft_substr(env->envp[j], size + 1, ft_strlen(env->envp[j]) - size);
	temp2 = ft_substr(str, size + 1, ft_strlen(str) - size - 1);
	final = ft_strjoin(temp, temp2);
	free(temp);
	free(temp2);
	return (final);
}

void	exec_command(char **args, t_env *env, int out_fd)
{
	char	*exec_path;
	char	*path;
	char	*temp;

	if (!args || !args[0])
		return ;
	path = find_path(env);
	if (ft_strchr(args[0], '/')) // if, yani eger, biri path ile yazmissa komutu burasi kontrol edip calistiriyo
	{
		exec_path = NULL;
		run_with_path(args, env, out_fd);
	}
	else
		// eger pathsiz yazilmissa command burasi isi eline aliyo ve pathi bulup calistiriyo
	{
		exec_path = find_exec(args[0], path, 0, 5);
		if (!exec_path)
		{
			if (args[0] && args[0][0] == '$')
			{
				temp = get_path(args[0], env);
				if (ft_strchr(args[0], '=') == 0)
				{
					printf("minishell: %s: is a directory\n", temp);
					env->exit_code = 126;
				}
				else
				{
					printf("minishell: %s: No such file or directory\n", temp);
					env->exit_code = 127;
				}
				DEBUG_PRINT(GRN "exit status: %d\n" RESET, env->exit_code);
				free(temp);
				return ;
			}
			printf("minishell: %s: command not found.\n", args[0]);
			env->exit_code = 127;
			DEBUG_PRINT(GRN "exit status: %d\n" RESET, env->exit_code);
			return ;
		}
		run_without_path(args, env, out_fd, exec_path);
	}
	DEBUG_PRINT(GRN "exit status: %d\n" RESET, env->exit_code);
	if (exec_path)
		free(exec_path);
	free(path);
}
