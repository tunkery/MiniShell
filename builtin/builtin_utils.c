/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: batuhan <batuhan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/20 13:41:55 by bolcay            #+#    #+#             */
/*   Updated: 2025/03/24 23:14:27 by batuhan          ###   ########.fr       */
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

void	run_builtin(char **args, t_env *env, char *line)
{
	if (builtin_check(args) == 1)
		run_env(args, env);
	else if (builtin_check(args) == 2)
		run_pwd(args);
	else if (builtin_check(args) == 3)
		run_echo(args, line);
	else if (builtin_check(args) == 4)
		run_export(args, env);
	else if (builtin_check(args) == 5)
		run_unset(args, env);
	else if (builtin_check(args) == 6)
		run_cd(args, env);
	else if (builtin_check(args) == 7)
		run_exit();
}
