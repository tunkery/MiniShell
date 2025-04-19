/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   run_commands.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: batuhan <batuhan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/25 13:26:21 by bolcay            #+#    #+#             */
/*   Updated: 2025/04/19 14:56:02 by batuhan          ###   ########.fr       */
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
			if (exe)
				free(exe);
			exit(127);
		}
	}
	else if (pid < 0)
		perror("fork Failed");
	else
		wait_for_child(pid, env);
}

// static void	run_with_path(char **args, t_env *env, int out_fd)
// {
// 	pid_t	pid;

// 	if (access(args[0], X_OK) == 0)
// 	{
// 		pid = fork();
// 		if (pid == 0)
// 		{
// 			if (out_fd != STDOUT_FILENO)
// 			{
// 				dup2(out_fd, STDOUT_FILENO);
// 				close(out_fd);
// 			}
// 			if (execve(args[0], args, env->envp) == -1)
// 			{
// 				perror("execvp Failed");
// 				exit(127);
// 			}
// 		}
// 		else if (pid < 0)
// 			perror("fork Failed");
// 		else
// 			wait_for_child(pid, env);
// 	}
// 	else
// 		printf("minishell: %s: command not found.\n", args[0]);
// }

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
	pid_t	pid;
	int		check;

	(void)out_fd;
	check = permission_check(str, env);
	if (access(str, X_OK) == 0 && check == 0)
	{
		pid = fork();
		if (pid == 0)
		{
			if (out_fd != STDOUT_FILENO)
			{
				dup2(out_fd, STDOUT_FILENO);
				close(out_fd);
			}
			if (execve(str, args, env->envp) == -1)
			{
				perror("execve Failed");
				exit(127);
			}
		}
		else if (pid < 0)
			perror("fork Failed");
		else
			wait_for_child(pid, env);
	}
	else if (check == 0)
	{
		fprintf(stderr, "minishell: %s: command not found.\n", str);
		env->exit_code = 127;
	}
}

// static char	*get_path(char *str, t_env *env)
// {
// 	int		i;
// 	int		j;
// 	int		size;
// 	char	*key;
// 	char	*temp;
// 	char	*temp2;
// 	char	*final;

// 	i = 1;
// 	j = 0;
// 	key = ft_substr(str, 1, key_size(str));
// 	size = ft_strlen(key);
// 	while (env->envp[j] && ft_strncmp(env->envp[j], key, size) != 0)
// 		j++;
// 	if (!env->envp[j])
// 		return (NULL);
// 	temp = ft_substr(env->envp[j], size + 1, ft_strlen(env->envp[j]) - size);
// 	temp2 = ft_substr(str, size + 1, ft_strlen(str) - size - 1);
// 	final = ft_strjoin(temp, temp2);
// 	free(temp);
// 	free(temp2);
// 	return (final);
// }

void	exec_command(char **args, t_env *env, int out_fd)
{
	char	*exec_path;
	char	*path;
	char	*final;

	if (!args || !args[0])
		return ;
	final = args[0];
	path = find_path(env);
	exec_path = find_exec(final, path, 0, 5);
	if (ft_strchr(final, '/')) // if, yani eger, biri path ile yazmissa komutu burasi kontrol edip calistiriyo
		run_with_path(final, args, env, out_fd);
	else
		// eger pathsiz yazilmissa command burasi isi eline aliyo ve pathi bulup calistiriyo
	{
		if (!exec_path)
		{
			env->exit_code = 127;
			fprintf(stderr, "minishell: %s: command not found.\n", args[0]);
		}
		else
			run_without_path(args, env, out_fd, exec_path);
	}
	if (exec_path)
		free(exec_path);
}
