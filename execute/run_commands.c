/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   run_commands.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: batuhan <batuhan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/25 13:26:21 by bolcay            #+#    #+#             */
/*   Updated: 2025/04/22 19:39:36 by batuhan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	wait_for_child(pid_t pid, t_env *env)
{
	int					status;
	struct sigaction	sa_ignore;

	sa_ignore.sa_handler = SIG_IGN;
	sigemptyset(&sa_ignore.sa_mask);
	sa_ignore.sa_flags = 0;
	sigaction(SIGQUIT, &sa_ignore, NULL);
	waitpid(pid, &status, 0);
	if (WIFEXITED(status))
		env->exit_code = WEXITSTATUS(status);
	else if (WIFSIGNALED(status))
	{
		env->exit_code = 128 + WTERMSIG(status);
		if (WTERMSIG(status) == SIGQUIT)
			write(STDERR_FILENO, "^\\Quit: 3\n", 10);
	}
	signal_mode_command();
}

void	run_without_path(char **args, t_env *env, int out_fd, char *exe)
{
	pid_t	pid;
	int		is_cat_command;

	is_cat_command = 0;
	if (args[0] && ft_strcmp(args[0], "cat") == 0 && (!args[1]
			|| args[1][0] == '-'))
		set_signal_backslash();
	pid = fork();
	if (pid == 0)
	{
		if (is_cat_command)
			if (out_fd != STDOUT_FILENO)
			{
				dup2(out_fd, STDOUT_FILENO);
				close(out_fd);
			}
		if (execve(exe, args, env->envp) == -1)
		{
			exit(127);
		}
	}
	else if (pid < 0)
		perror("fork Failed");
	else
		wait_for_child(pid, env);
}

void	exec_command_path(char **args, t_env *env, int out_fd, char *exec_path)
{
	char	*final;

	final = args[0];
	if (ft_strchr(final, '/'))
		run_with_path(final, args, env, out_fd);
	else
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
}

void	exec_command(char **args, t_env *env, int out_fd)
{
	char				*exec_path;
	char				*path;
	int					is_cat_command;
	struct sigaction	old_quit;
	struct sigaction	sa_current;

	is_cat_command = 0;
	if (!args || !args[0])
		return ;
	sigaction(SIGQUIT, NULL, &old_quit);
	if (args[0] && ft_strcmp(args[0], "cat") == 0 && (!args[1]
			|| args[1][0] == '-'))
		is_cat_command = 1;
	// final = args[0];
	path = find_path(env);
	exec_path = find_exec(args[0], path, 0, env);
	gc_register(env->s_gc, exec_path);
	if (is_cat_command)
		set_signal_backslash();
	exec_command_path(args, env, out_fd, exec_path);
	sigaction(SIGQUIT, &old_quit, NULL);
}
