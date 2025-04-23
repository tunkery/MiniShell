/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   envp_utils2.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bolcay <bolcay@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/01 11:18:02 by bolcay            #+#    #+#             */
/*   Updated: 2025/04/23 16:56:46 by bolcay           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	*find_path(t_env *env)
{
	int	i;

	i = 0;
	while (env->envp[i] && ft_strncmp(env->envp[i], "PATH", 4) != 0)
		i++;
	if (!env->envp[i])
		return (NULL);
	return (env->envp[i]);
}

char	*copy_ex_helper(char *str)
{
	int		size;
	char	*key;
	char	*value;
	char	*result;

	size = key_size(str) + 1;
	key = ft_substr(str, 0, size + 1);
	value = ft_substr_ex(str, size, ft_strlen(str) - size);
	result = ft_strjoin(key, value);
	if (key)
		free(key);
	if (value)
		free(value);
	if (!result)
		return (NULL);
	return (result);
}

void	copy_ex(char **str, char ***envp, t_env *env)
{
	int		i;
	int		size;
	char	*temp;

	if (!str || !envp)
		return ;
	i = 0;
	size = env_size(str);
	*envp = my_malloc(env->gc, sizeof(char *) * (size + 1));
	if (!*envp)
		return ;
	while (i < size)
	{
		temp = copy_ex_helper(str[i]);
		(*envp)[i] = ft_strdup(temp);
		gc_register(env->gc, (*envp)[i]);
		if (temp)
			free(temp);
		i++;
	}
	(*envp)[i] = NULL;
}
