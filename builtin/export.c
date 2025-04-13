/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bolcay <bolcay@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/23 08:59:18 by bolcay            #+#    #+#             */
/*   Updated: 2025/04/13 18:58:01 by bolcay           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static int	name_check(char *args)
{
	// printf("%s\n", args);
	if (!args)
		return (-1);
	if (ft_strchr(args, '-') != 0)
		return (-2);
	if (ft_strchr(args, ' ') != 0)
		return (-1);
	if (ft_strchr(args, 9) != 0)
		return (-1);
	if (ft_strchr(args, 10) != 0)
		return (-1);
	if (ft_strchr(args, 11) != 0)
		return (-1);
	if (ft_strchr(args, 12) != 0)
		return (-1);
	if (ft_strchr(args, 13) != 0)
		return (-1);
	if (ft_isdigit(args[0]) != 0)
		return (-1);
	if (args[0] == '=')
		return (-1);
	return (0);
}

static int	duplicate_check_env(char *args, t_env *env)
{
	int		i;
	int		size;

	size = key_size(args);
	i = 0;
	while (env->envp[i])
	{
		if (ft_strncmp(env->envp[i], args, size) == 0)
		{
			printf("%s\n", env->envp[i]);
			return (0);
		}
		i++;
	}
	if (!env->envp[i])
		return (1);
	return (0);
}

static int	duplicate_check(char *args, t_env *env)
{
	int		i;
	int		size;

	size = key_size(args);
	i = 0;
	while (env->export[i])
	{
		if (ft_strncmp(env->export[i], args, size) == 0)
		{
			printf("%s\n", env->export[i]);
			return (0);
		}
		i++;
	}
	if (!env->export[i])
		return (1);
	return (0);
}

static void	duplicate_fix_env(char *str, t_env *env)
{
	int	i;
	int	size;

	i = 0;
	size = key_size(str);
	while (env->envp[i])
	{
		if (ft_strncmp(env->envp[i], str, size) == 0)
		{
			free(env->envp[i]);
			env->envp[i] = ft_strdup(str);
			break ;
		}
		i++;
	}
	env->exit_code = 0;
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
	i = 0;
	while (env->envp[i])
		i++;
	if (!env->envp[i])
	{
		if (duplicate_check_env(str, env) == 0)
			duplicate_fix_env(str, env);
		else
			env->envp = update_env(env->envp, str);
	}
	env->exit_code = 0;
}

void	run_export(char **args, t_env *env)
{
	int	i;
	int	j;
	int	check;

	i = 0;
	j = 1;
	check = 0;
	if (!args[1])
	{
		while (env->export[i])
		{
			printf("declare -x ");
			printf("%s\n", env->export[i++]);
		}
		env->exit_code = 0;
		return ;
	}
	while (args[j])
	{
		if (name_check(args[j]) == -1)
		{
			printf("minishell: export: '%s': not a valid identifier\n", args[j]);
			env->exit_code = 1;
			check = 1;
		}
		else if (name_check(args[j]) == -2)
		{
			printf("minishell: export: %s: invalid option", args[j]);
			env->exit_code = 2;
			check = 2;
		}
		else if (ft_strchr(args[j], '=') != 0 && duplicate_check(args[j], env) == 0 &&
				duplicate_check_env(args[j], env) == 0)
			{
				duplicate_fix(args[j], env);
				duplicate_fix_env(args[j], env);
			}		
		else if (ft_strchr(args[j], '=') != 0 && duplicate_check(args[j], env) == 0)
			duplicate_fix(args[j], env);
		else if (ft_strchr(args[j], '=') != 0)
		{
			env->envp = update_env(env->envp, args[j]);
			env->export = update_env(env->export, args[j]);
		}
		else
			env->export = update_env(env->export, args[j]);
		j++;
	}
	env->exit_code = check;
	// DEBUG_PRINT(GRN "exit status: %d\n" RESET, env->exit_code);
}
