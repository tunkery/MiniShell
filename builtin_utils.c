/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bolcay <bolcay@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/20 13:41:55 by bolcay            #+#    #+#             */
/*   Updated: 2025/03/20 16:11:26 by bolcay           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	builtin_check(char **tokens)
{
	if (ft_strncmp(tokens[0], "env", ft_strlen(tokens[0])) == 0)
		return (1);
	if (ft_strncmp(tokens[0], "pwd", ft_strlen(tokens[0])) == 0)
		return (2);
	if (ft_strncmp(tokens[0], "echo", ft_strlen(tokens[0])) == 0)
	{
		if (ft_strncmp(tokens[1], "-n", ft_strlen(tokens[1])) == 0)
		{
		}
		else if (tokens[1] != NULL)
		{
		}
		return (3);
	}
	if (ft_strncmp(tokens[0], "export", ft_strlen(tokens[0])) == 0)
	{
		return (4);
	}
	if (ft_strncmp(tokens[0], "unset", ft_strlen(tokens[0])) == 0)
	{
		return (5);
	}
	if (ft_strncmp(tokens[0], "cd", ft_strlen(tokens[0])) == 0)
	{
		return (6);
	}
	if (ft_strncmp(tokens[0], "exit", ft_strlen(tokens[0])) == 0)
	{
		if (tokens[1] != NULL)
		{
		}
	}
	return (0);
}

void	run_builtin(char **args, t_env *env)
{
	int check;

	check = 0;
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
		env->envp = update_env(env->envp, args[1]);
	}
	else if (builtin_check(args) == 5)
	{
		env->envp = remove_env(env->envp, args[1]);
	}
}
