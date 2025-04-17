/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_utils2.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bolcay <bolcay@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/16 13:25:02 by batuhan           #+#    #+#             */
/*   Updated: 2025/04/17 13:09:39 by bolcay           ###   ########.fr       */
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
	char	*temp;
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
	if (env->export && env->export[i])
	{
		temp = ft_strjoin(env->export[i], value);
		free(env->export[i]);
		env->export[i] = ft_strdup(temp);
		free(temp);
		free(value);
		free(key);
	}
	else
	{
		temp = append_organiser(str);
		if (value)
			free(value);
		if (key)
			free(key);
		env->export = update_env(env->export, temp);
		free(temp);
	}
}

void	append_env(char *str, t_env *env)
{
	int	i;
	int	size;
	char	*temp;
	char	*value;
	char	*key;

	i = 0;
	size = 0;
	printf("%s\n", str);
	while (str[size] && str[size] != '=')
		size++;
	value = ft_substr(str, size + 1, ft_strlen(str) - size - 1);
	printf("%s\n", value);
	size = 0;
	while (str[size] && str[size] != '+')
		size++;
	printf("%d\n", size);
	printf("%d\n", append_key_size(str));
	key = ft_substr(str, 0, size);
	printf("%s\n", key);
	while (env->envp && env->envp[i])
	{
		if (ft_strncmp(env->envp[i], key, size) == 0)
		{
			if (size == append_key_size(str) + 1)
				break ;
		}
		i++;
	}
	if (env->envp[i])
		printf("%s\n", env->envp[i]);
	if (env->envp && env->envp[i])
	{
		printf("hii\n");
		temp = ft_strjoin(env->envp[i], value);
		free(env->envp[i]);
		env->envp[i] = ft_strdup(temp);
		free(temp);
		free(value);
		free(key);
	}
	else
	{
		printf("hii2\n");
		temp = append_organiser(str);
		if (value)
			free(value);
		if (key)
			free(key);
		env->envp = update_env(env->envp, temp);
		free(temp);
	}
}
