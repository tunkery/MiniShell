/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   envp_utils2.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bolcay <bolcay@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/01 11:18:02 by bolcay            #+#    #+#             */
/*   Updated: 2025/04/01 11:50:15 by bolcay           ###   ########.fr       */
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

static int	skip_entry(char *entry, char *key)
{
	int	key_len;

	key_len = ft_strlen(key);
	return (ft_strncmp(entry, key, key_len) == 0 && (entry[key_len] == '='
			|| entry[key_len] == '\0'));
}

char	**copy_filtred_env(char **envp, char *key, t_env *env, int size)
{
	int		i;
	int		j;
	char	**new_env;

	i = 0;
	j = 0;
	new_env = my_malloc(env->gc, sizeof(char *) * size);
	if (!new_env)
		return (NULL);
	while (envp[i])
	{
		if (skip_entry(envp[i], key))
		{
			gc_unregister(env->gc, envp[i]);
			free(envp[i++]);
			continue ;
		}
		if (envp[i])
		{
			new_env[j++] = ft_strdup(envp[i++]);
			gc_register(env->gc, new_env[j - 1]);
		}
	}
	new_env[j] = NULL;
	return (new_env);
}

char	**remove_env(char **envp, char *key, t_env *env)
{
	int	size;

	size = env_size(envp);
	return (copy_filtred_env(envp, key, env, size));
}
