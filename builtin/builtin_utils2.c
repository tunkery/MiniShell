/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_utils2.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bolcay <bolcay@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/16 13:25:02 by batuhan           #+#    #+#             */
/*   Updated: 2025/04/17 13:25:26 by bolcay           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	append_check(char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (str[i] == '+')
		{
			i++;
			if (str[i] && str[i] == '=')
				return (1);
			else
				return (0);
		}
		i++;
	}
	return (0);
}

char	*append_organiser(char *str)
{
	int	i;
	char	*temp;
	char	*temp1;
	char	*result;

	i = 0;
	while (str[i] && str[i] != '+')
		i++;
	temp = ft_substr(str, 0, i);
	i += 1;
	temp1 = ft_substr(str, i, ft_strlen(str) - i);
	result = ft_strjoin(temp, temp1);
	free(temp);
	free(temp1);
	return (result);
}

int	append_key_size(char *str)
{
	int i;

	i = 0;
	while (str[i])
	{
		if (str[i] == '+')
			return (i - 1);
		i++;
	}
	return (i);
}

void	append_exp(char *str, t_env *env)
{
	int	i;
	int	size;
	char	*value;
	char	*key;

	i = 0;
	size = 0;
	while (str[size] && str[size] != '=')
		size++;
	value = ft_substr(str, size + 1, ft_strlen(str) - size - 1);
	size = 0;
	while (str[size] && str[size] != '+')
		size++;
	key = ft_substr(str, 0, size);
	while (env->export && env->export[i])
	{
		if (ft_strncmp(env->export[i], key, size) == 0)
		{
			if (size == append_key_size(str) + 1)
				break ;
		}
		i++;
	}
	append1(str, env, value, i);
}

void	append1(char *str, t_env *env, char *value, int i)
{
	char	*temp;

	if (env->export[i])
	{
		temp = ft_strjoin(env->export[i], value);
		free(env->export[i]);
		env->export[i] = ft_strdup(temp);
		free(temp);
	}
	else
	{
		temp = append_organiser(str);
		env->export = update_env(env->export, temp);
		free(temp);
	}
}

void	append(char *str, t_env *env, char *value, int i)
{
	char	*temp;

	if (env->envp[i])
	{
		temp = ft_strjoin(env->envp[i], value);
		free(env->envp[i]);
		env->envp[i] = ft_strdup(temp);
		free(temp);
	}
	else
	{
		temp = append_organiser(str);
		env->envp = update_env(env->envp, temp);
		free(temp);
	}
}

void	append_env(char *str, t_env *env)
{
	int	i;
	int	size;
	char	*value;
	char	*key;

	i = 0;
	size = 0;
	while (str[size] && str[size] != '=')
		size++;
	value = ft_substr(str, size + 1, ft_strlen(str) - size - 1);
	size = 0;
	while (str[size] && str[size] != '+')
		size++;
	key = ft_substr(str, 0, size);
	while (env->envp && env->envp[i])
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
