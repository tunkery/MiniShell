/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   envp.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bolcay <bolcay@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 12:08:19 by bolcay            #+#    #+#             */
/*   Updated: 2025/03/17 12:21:07 by bolcay           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	**update_env(char **envp, char *key)
{
	int i;
	int size;
	char **new_env;
	
	i = 0;
	size = env_size(envp) + 1;
	new_env = malloc(sizeof(char **) * size);
	if (!new_env)
		return (NULL);
	copy_env(envp, new_env);
	new_env[size] = ft_strdup(key);
	clean_2d(envp);
	return (new_env);
}

char	**remove_env(char **envp, char *key)
{
	int i;
	int j;
	int size;
	char **new_env;

	i = 0;
	j = 0;
	size = env_size(envp) - 1;
	new_env = malloc(sizeof(char **) * size);
	if (!new_env)
		return (NULL);
	while (envp[i])
	{
		if (ft_strncmp(envp[i], key, ft_strlen(key)) == 0)
			i++;
		new_env[j] = ft_strdup(envp[i]);
		i++;
		j++;
	}
	clean_2d(envp);
	return (new_env);
}
