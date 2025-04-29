/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   run_command1.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hpehliva <hpehliva@student.42heilbronn.de  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/30 00:04:31 by hpehliva          #+#    #+#             */
/*   Updated: 2025/04/30 00:04:33 by hpehliva         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	permision_denied(char *ptr)
{
	write(2, "minishell: ", 11);
	write(2, ptr, ft_strlen(ptr));
	write(2, ": Permission denied\n", 20);
}

void	no_such_directory(char *ptr)
{
	write(2, "minishell: ", 11);
	write(2, ptr, ft_strlen(ptr));
	write(2, ": No such file or directory\n", 28);
}

void	is_directory(char *ptr)
{
	write(2, "minishell: ", 11);
	write(2, ptr, ft_strlen(ptr));
	write(2, ": is a directory\n", 17);
}

int	permission_check(char *str, t_env *env)
{
	struct stat	info;

	if (stat(str, &info) != 0)
	{
		no_such_directory(str);
		env->exit_code = 127;
		return (1);
	}
	else if (S_ISDIR(info.st_mode))
	{
		is_directory(str);
		env->exit_code = 126;
		return (1);
	}
	else if (access(str, X_OK) != 0)
	{
		permision_denied(str);
		env->exit_code = 126;
		return (1);
	}
	return (0);
}

void	run_with_path(char *str, char **args, t_env *env, int out_fd)
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
		write(2, args[0], ft_strlen(args[0]));
		write(2, ": command not found.\n", 21);
		env->exit_code = 127;
	}
}
