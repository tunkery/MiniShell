/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: batuhan <batuhan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/23 08:59:18 by bolcay            #+#    #+#             */
/*   Updated: 2025/04/11 13:37:16 by batuhan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static int	name_check(char **args)
{
	if (ft_strchr(args[1], '-') != 0)
		return (-1);
	if (ft_strchr(args[1], ' ') != 0)
		return (-1);
	if (ft_strchr(args[1], 9) != 0)
		return (-1);
	if (ft_strchr(args[1], 10) != 0)
		return (-1);
	if (ft_strchr(args[1], 11) != 0)
		return (-1);
	if (ft_strchr(args[1], 12) != 0)
		return (-1);
	if (ft_strchr(args[1], 13) != 0)
		return (-1);
	if (ft_isdigit(args[1][0]) != 0)
		return (-1);
	return (0);
}

static int	duplicate_check(char **args, t_env *env)
{
	int	i;
	
	i = 0;
	while (env->export[i] && ft_strncmp(env->export[i], args[1], ft_strlen(args[1])) == 0)
		i++;
	if (!env->export[i])
		return (1);
	return (0);
}

static void	duplicate_fix(char *str, t_env *env)
{
	int	i;
	int	size;

	i = 0;
	size = key_size(str);
	while (env->export[i])
	{
		if (ft_strncmp(env->export[i], str, size) == 0)
		{
			free(env->export[i]);
			env->export[i] = ft_strdup(str);
			break ;
		}
		i++;
	}
	env->envp = update_env(env->envp, str);
	env->exit_code = 0;
}

void	run_export(char **args, t_env *env)
{
	int	i;

	i = 0;

	if (!args[1])
	{
		while (env->export[i])
		{
			printf("declare -x ");
			printf("%s\n", env->export[i++]);
		}
		env->exit_code = 0;
	}
	else if (name_check(args) != 0)
		printf("minishell: export: '%s': not a valid identifier\n", args[1]);
	else if (ft_strchr(args[1], '=') != 0 && duplicate_check(args, env) == 0)
		duplicate_fix(args[1], env);
	else if (ft_strchr(args[1], '=') != 0)
	{
		env->envp = update_env(env->envp, args[1]);
		env->export = update_env(env->export, args[1]);
		env->exit_code = 0;
	}
	else
	{
		env->export = update_env(env->export, args[1]);
		env->exit_code = 0;
	}
	DEBUG_PRINT(GRN "exit status: %d\n" RESET, env->exit_code);
}
