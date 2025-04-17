/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_utils3.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bolcay <bolcay@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/16 13:31:23 by batuhan           #+#    #+#             */
/*   Updated: 2025/04/17 13:41:34 by bolcay           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	duplicate_check_env(char *str, t_env *env)
{
	int		i;
	int		size;

	size = key_size(str);
	i = 0;
	while (env->envp && env->envp[i])
	{
		if (ft_strncmp(env->envp[i], str, size) == 0)
		{
			if (size != key_size(env->envp[i]))
				i++;
			else
				return (0);
		}
		if (env->envp[i])
			i++;
		else
			break ;
	}
	if (!env->envp)
		return (1);
	if (!env->envp[i])
		return (1);
	return (0);
}

int	duplicate_check_ex(char *args, t_env *env)
{
	int		i;
	int		size;

	size = key_size(args);
	i = 0;
	while (env->export && env->export[i])
	{
		if (ft_strncmp(env->export[i], args, size) == 0)
		{
			if (size != key_size(env->export[i]))
				i++;
			else
				return (0);
		}
		if (env->export[i])
			i++;
		else
			break ;
	}
	if (!env->export)
		return (1);
	if (!env->export[i])
		return (1);
	return (0);
}

void	duplicate_fix_env(char *str, t_env *env)
{
	int	i;
	int	size;

	i = 0;
	size = key_size(str);
	while (env->envp[i])
	{
		if (ft_strncmp(env->envp[i], str, size) == 0)
		{
			if(env->envp[i])
				free(env->envp[i]);
			env->envp[i] = ft_strdup(str);
			break ;
		}
		i++;
	}
	env->exit_code = 0;
}

void	duplicate_fix_ex(char *str, t_env *env)
{
	int	i;
	int	size;

	i = 0;
	size = key_size(str);
	while (env->export && env->export[i])
	{
		if (ft_strncmp(env->export[i], str, size) == 0)
		{
			if (env->export[i])
				free(env->export[i]);
			env->export[i] = ft_strdup(str);
			break ;
		}
		i++;
	}
	if (duplicate_check_env(str, env) == 0)
		duplicate_fix_env(str, env);
	else
		env->envp = update_env(env->envp, str);
	env->exit_code = 0;
}

void	export1(char *str, t_env *env)
{
	if (ft_strchr(str, '=') != 0)
	{
		if (duplicate_check_ex(str, env) == 0 && duplicate_check_env(str, env) == 0)
		{
			duplicate_fix_ex(str, env);
			duplicate_fix_env(str, env);
		}
		else if (duplicate_check_ex(str, env) == 0)
			duplicate_fix_ex(str, env);
		else
		{
			env->envp = update_env(env->envp, str);
			env->export = update_env(env->export, str);
		}
	}
	else
		env->export = update_env(env->export, str);
}

void	checker(char *str, int name_c, int *check, t_env *env)
{
	if (name_c == -1)
	{
		fprintf(stderr, "minishell: export: '%s': not a valid identifier\n", str);
		env->exit_code = 1;
		*check = 1;
	}
	else if (name_c == -2)
	{
		fprintf(stderr, "minishell: export: %s: invalid option\n", str);
		env->exit_code = 2;
		*check = 2;
	}
	else if (name_c == -3)
	{
		append_env(str, env);
		append_exp(str, env);
	}
}
