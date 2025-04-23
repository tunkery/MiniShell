/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_utils2.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bolcay <bolcay@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/16 13:25:02 by batuhan           #+#    #+#             */
/*   Updated: 2025/04/23 16:42:10 by bolcay           ###   ########.fr       */
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
	int		i;
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
	int	i;

	i = 0;
	while (str[i])
	{
		if (str[i] == '+')
			return (i - 1);
		i++;
	}
	return (i);
}

void	append_exp(char *str, t_env *env, int i)
{
	int		size;
	char	*value;
	char	*key;

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

char	*append2(char *str, char *value)
{
	int		i;
	char	*temp;
	char	*temp2;
	char	*temp3;

	i = 0;
	temp = ft_substr(str, 0, ft_strlen(str) - 1);
	temp2 = ft_strjoin(temp, value);
	free(temp);
	temp3 = malloc(sizeof(char) * ft_strlen(temp2) + 3);
	while (temp2[i])
	{
		temp3[i] = temp2[i];
		i++;
	}
	free(temp2);
	temp3[i++] = '"';
	temp3[i] = '\0';
	return (temp3);
}
