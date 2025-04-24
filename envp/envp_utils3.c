/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   envp_utils3.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bolcay <bolcay@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/23 16:59:45 by bolcay            #+#    #+#             */
/*   Updated: 2025/04/24 18:46:20 by bolcay           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	**update_env(char **envp, char *key, t_env *env)
{
	int		size;
	char	**new_env;
	int		i;

	if (!envp || !key)
		return (NULL);
	i = 0;
	size = env_size(envp);
	new_env = NULL;
	new_env = my_malloc(env->gc, sizeof(char *) * (size + 2));
	if (!new_env)
		return (NULL);
	while (envp[i])
	{
		new_env[i] = ft_strdup(envp[i]);
		gc_register(env->gc, new_env[i]);
		i++;
	}
	new_env[size] = ft_strdup(key);
	gc_register(env->gc, new_env[size]);
	new_env[size + 1] = NULL;
	return (new_env);
}

char	**update_ex(char **envp, char *key, t_env *env)
{
	int		size;
	char	**new_env;
	int		i;
	char	*temp;

	if (!envp || !key)
		return (NULL);
	i = 0;
	size = env_size(envp);
	new_env = NULL;
	new_env = my_malloc(env->gc, sizeof(char *) * (size + 2));
	if (!new_env)
		return (NULL);
	while (envp[i])
	{
		new_env[i] = ft_strdup(envp[i]);
		gc_register(env->gc, new_env[i]);
		i++;
	}
	temp = copy_ex_helper(key);
	new_env[size] = ft_strdup(temp);
	gc_register(env->gc, new_env[size]);
	free(temp);
	new_env[size + 1] = NULL;
	return (new_env);
}

char	**remove_env(char **envp, char *key, t_env *env)
{
	int		i;
	int		j;
	int		size;
	char	**new_env;

	i = 0;
	j = 0;
	size = env_size(envp);
	new_env = my_malloc(env->gc, sizeof(char *) * (size + 1));
	if (!new_env)
		return (NULL);
	while (envp[i])
	{
		if (ft_strncmp(envp[i], key, ft_strlen(key)) == 0
			&& (envp[i][ft_strlen(key)] == '='
			|| envp[i][ft_strlen(key)] == '\0'))
		{
			gc_unregister(env->gc, envp[i]);
			free(envp[i++]);
		}
		if (envp[i])
		{
			new_env[j] = ft_strdup(envp[i]);
			gc_register(env->gc, new_env[j]);
			i++;
			j++;
		}
	}
	new_env[j] = NULL;
	return (new_env);
}

static char	*ft_gnls_substr(char const *s, unsigned int start, size_t len,
	t_env *env)
{
	char	*new_s;
	size_t	i;

	if (!s)
		return (NULL);
	i = 0;
	if (start >= ft_gnl_strlen(s))
	{
		new_s = ft_gnl_strdup("");
		gc_register(env->s_gc, new_s);
		return (new_s);
	}
	if (len > ft_gnl_strlen(s) - start)
		len = ft_gnl_strlen(s) - start;
	new_s = (char *)malloc(len + 2);
	if (!new_s)
		return (NULL);
	while (i < len && s[start + i] != ':')
	{
		new_s[i] = s[start + i];
		i++;
	}
	new_s[i++] = '/';
	new_s[i] = '\0';
	return (gc_register(env->s_gc, new_s), new_s);
}

char	*find_exec(char *command, char *path, int i, t_env *env)
{
	char	*temp;
	int		j;

	if (!command || !path)
		return (NULL);
	j = 5;
	while (path[i])
	{
		while (path[i] && path[i] && path[i] != ':')
			i++;
		temp = ft_gnls_substr(path, j, i - 4, env);
		if (!temp)
			return (NULL);
		temp = ft_strjoin(temp, command);
		if (access(temp, X_OK) == 0)
			return (gc_register(env->s_gc, temp), temp);
		free(temp);
		temp = NULL;
		if (!path[i])
			break ;
		j = i + 1;
		i++;
	}
	return (temp);
}
