/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_utils3.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bolcay <bolcay@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/16 13:31:23 by batuhan           #+#    #+#             */
/*   Updated: 2025/04/23 16:45:30 by bolcay           ###   ########.fr       */
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
		if (ft_strncmp(env->envp[i], str, size) == 0
			&& env->envp[i][size + 1] == '=' && key_size(env->envp[i]) == size
			&& env->envp[i][0] == str[0])
		{
			return (0);
		}
		if (env->envp[i])
			i++;
		else
			break ;
	}
	if (!env->envp || !env->envp[i])
		return (1);
	return (0);
}

int	duplicate_check_ex(char *str, t_env *env)
{
	int		i;
	int		size;

	size = key_size(str);
	i = 0;
	while (env->export && env->export[i])
	{
		if (ft_strncmp(env->export[i], str, size) == 0
			&& env->export[i][size + 1] == '='
			&& key_size(env->export[i]) == size
			&& env->export[i][0] == str[0])
		{
			return (0);
		}
		if (env->export[i])
			i++;
		else
			break ;
	}
	if (!env->export || !env->export[i])
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
			if (size != key_size(env->envp[i]))
				i++;
			else
			{
				env->envp[i] = ft_strdup(str);
				gc_register(env->gc, env->envp[i]);
				break ;
			}
		}
		i++;
	}
	env->exit_code = 0;
}

void	duplicate_fix_ex(char *str, t_env *env, int i, int size)
{
	char	*temp;

	while (env->export && env->export[i])
	{
		if (ft_strncmp(env->export[i], str, size) == 0)
		{
			if (size != key_size(env->export[i]))
				i++;
			else
			{
				temp = copy_ex_helper(str);
				env->export[i] = ft_strdup(temp);
				gc_register(env->gc, env->export[i]);
				free(temp);
				break ;
			}
		}
		i++;
	}
	if (duplicate_check_env(str, env) == 0)
		duplicate_fix_env(str, env);
	else
		env->envp = update_env(env->envp, str, env);
	env->exit_code = 0;
}

void	export1(char *str, t_env *env)
{
	if (ft_strchr(str, '=') != 0)
	{
		if (duplicate_check_ex(str, env) == 0
			&& duplicate_check_env(str, env) == 0)
		{
			duplicate_fix_ex(str, env, 0, 0);
			duplicate_fix_env(str, env);
		}
		else if (duplicate_check_ex(str, env) == 0)
		{
			duplicate_fix_ex(str, env, 0, 0);
		}
		else
		{
			env->envp = update_env(env->envp, str, env);
			env->export = update_ex(env->export, str, env);
		}
	}
	else
	{
		env->export = update_ex(env->export, str, env);
	}
}
