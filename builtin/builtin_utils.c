/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: batuhan <batuhan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/20 13:41:55 by bolcay            #+#    #+#             */
/*   Updated: 2025/04/29 19:36:59 by batuhan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	key_check(char *args)
{
	int	i;

	i = 0;
	if (ft_isalpha(args[0]) != 1 && args[0] != '_')
		return (-1);
	while (args[i])
	{
		if (ft_isalnum(args[i]) != 1 && args[i] != '_')
			return (-1);
		i++;
	}
	return (0);
}

int	builtin_check(char **tokens)
{
	int	check;

	check = ft_strlen(tokens[0]);
	if (!tokens || !tokens[0])
		return (0);
	if (ft_strncmp(tokens[0], "env", 3) == 0 && check == 3)
		return (1);
	if (ft_strncmp(tokens[0], "pwd", 3) == 0 && check == 3)
		return (2);
	if (ft_strncmp(tokens[0], "echo", 4) == 0 && check == 4)
		return (3);
	if (ft_strncmp(tokens[0], "export", 6) == 0 && check == 6)
		return (4);
	if (ft_strncmp(tokens[0], "unset", 5) == 0 && check == 5)
		return (5);
	if (ft_strncmp(tokens[0], "cd", 2) == 0 && check == 2)
		return (6);
	if (ft_strncmp(tokens[0], "exit", 4) == 0 && check == 4)
		return (7);
	return (0);
}

void	run_builtin(char **args, t_env *env)
{
	if (builtin_check(args) == 1)
		run_env(args, env);
	else if (builtin_check(args) == 2)
		run_pwd(args, env);
	else if (builtin_check(args) == 3)
		run_echo(args, env);
	else if (builtin_check(args) == 4)
		run_export(args, env);
	else if (builtin_check(args) == 5)
		run_unset(args, env);
	else if (builtin_check(args) == 6)
		run_cd(args, env);
	else if (builtin_check(args) == 7)
		run_exit(args, env);
}
