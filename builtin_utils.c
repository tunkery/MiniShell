/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bolcay <bolcay@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/20 13:41:55 by bolcay            #+#    #+#             */
/*   Updated: 2025/03/23 08:24:54 by bolcay           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	cd_util(char **args)
{
	if (!args[1])
		return (-1);
	if (chdir(args[1]) != 0)
		return (-1);
	return (0);
}

int	builtin_check(char **tokens)
{
	if (ft_strncmp(tokens[0], "env", ft_strlen(tokens[0])) == 0)
		return (1);
	if (ft_strncmp(tokens[0], "pwd", ft_strlen(tokens[0])) == 0)
		return (2);
	if (ft_strncmp(tokens[0], "echo", ft_strlen(tokens[0])) == 0)
		return (3);
	if (ft_strncmp(tokens[0], "export", ft_strlen(tokens[0])) == 0)
		return (4);
	if (ft_strncmp(tokens[0], "unset", ft_strlen(tokens[0])) == 0)
		return (5);
	if (ft_strncmp(tokens[0], "cd", ft_strlen(tokens[0])) == 0)
		return (6);
	if (ft_strncmp(tokens[0], "exit", ft_strlen(tokens[0])) == 0)
		return (7);
	return (0);
}

void	run_builtin(char **args, t_env *env)
{
	int check;
	int	i;

	check = 0;
	i = 0;
	if (builtin_check(args) == 1)
	{	
		if (args[1])
		{
			printf("env: illegal option %s\n", args[1]);
			printf("usage: env\n");
			return ;
		}
		cell_env(env);
	}
	else if (builtin_check(args) == 2)
	{
		if (args[1] && args[1][0] == '-')
		{
			printf("minishell: pwd: %s: invalid option\n", args[1]);
			printf("pwd: usage: pwd\n");
			return ;
		}
		cell_pwd();
	}
	else if (builtin_check(args) == 3)
	{
		if (ft_strncmp(args[1], "-n", ft_strlen(args[1])) == 0)
			check++;
		cell_echo(args, check);
	}
	else if (builtin_check(args) == 4)
	{
		if (!args[1])
		{
			while (env->export[i])
			{
				printf("%s\n", env->export[i]);
				i++;
			}
		}
		else if (ft_strchr(args[1], '=') != 0)
		{
			env->envp = update_env(env->envp, args[1]);
			env->export = update_env(env->export, args[1]);
		}
		else
			env->export = update_env(env->export, args[1]);
	}
	else if (builtin_check(args) == 5)
	{
		env->envp = remove_env(env->envp, args[1]);
		env->export = remove_env(env->export, args[1]);
	}
	else if (builtin_check(args) == 6)
	{
		cd_util(args);
	}
	else if (builtin_check(args) == 7)
	{
		exit(1);
	}
}
