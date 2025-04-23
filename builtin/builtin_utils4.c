/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_utils4.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bolcay <bolcay@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/23 14:40:10 by bolcay            #+#    #+#             */
/*   Updated: 2025/04/23 14:42:44 by bolcay           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	checker(char *str, int name_c, int *check, t_env *env)
{
	if (name_c == -1)
	{
		write(2, "minishell: export: '", 20);
		write(2, str, ft_strlen(str));
		write(2, "': not a valid identifier\n", 26);
		env->exit_code = 1;
		(*check) = 1;
	}
	else if (name_c == -2)
	{
		write(2, "minishell: export: ", 19);
		write(2, str, ft_strlen(str));
		write(2, ": invalid option\n", 17);
		env->exit_code = 2;
		(*check) = 2;
	}
	else if (name_c == -3)
	{
		append_env(str, env);
		append_exp(str, env);
	}
}

void	append_env(char *str, t_env *env)
{
	int		i;
	int		size;
	char	*value;
	char	*key;

	i = 0;
	size = 0;
	while (str[size] && str[size] != '=')
		size++;
	value = ft_substr(str, size + 1, ft_strlen(str) - size - 1);
	gc_register(env->gc, value);
	size = 0;
	while (str[size] && str[size] != '+')
		size++;
	key = ft_substr(str, 0, size);
	gc_register(env->gc, key);
	while (env->envp && env->envp[i++])
	{
		if (ft_strncmp(env->envp[i], key, size) == 0)
		{
			if (size == append_key_size(str) + 1)
				break ;
		}
		i++;
	}
	append(str, env, value, i);
}

void	append1(char *str, t_env *env, char *value, int i)
{
	char	*temp;

	if (env->export && env->export[i])
	{
		temp = append2(env->export[i], value);
		free(env->export[i]);
		env->export[i] = ft_strdup(temp);
		free(temp);
	}
	else
	{
		temp = append_organiser(str);
		env->export = update_ex(env->export, temp, env);
		free(temp);
	}
}

void	append(char *str, t_env *env, char *value, int i)
{
	char	*temp;

	if (env->envp && env->envp[i])
	{
		temp = ft_strjoin(env->envp[i], value);
		env->envp[i] = ft_strdup(temp);
		gc_register(env->gc, env->envp[i]);
		free(temp);
	}
	else
	{
		temp = append_organiser(str);
		env->envp = update_env(env->envp, temp, env);
		free(temp);
	}
}
