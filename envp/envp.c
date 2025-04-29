/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   envp.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: batuhan <batuhan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 12:08:19 by bolcay            #+#    #+#             */
/*   Updated: 2025/04/29 19:41:53 by batuhan          ###   ########.fr       */
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

void	setup_pwd_variables(t_env *env)
{
	int	i;

	i = 0;
	while (env->envp[i] && ft_strncmp(env->envp[i], "PWD", 3) != 0)
		i++;
	env->curr_pwd = ft_strdup(env->envp[i]);
	if (env->curr_pwd)
		gc_register(env->gc, env->curr_pwd);
	i = 0;
	while (env->envp[i] && ft_strncmp(env->envp[i], "OLDPWD", 6) != 0)
		i++;
	if (env->envp[i] && (ft_strncmp(env->envp[i], "OLDPWD", 6) == 0))
	{
		env->old_pwd = ft_strdup(env->envp[i]);
		if (!env->envp)
			return ;
		gc_register(env->gc, env->old_pwd);
	}
}

char	*ft_gnls_substr(char const *s, unsigned int start, size_t len,
		t_env *env)
{
	char	*new_s;
	size_t	i;
	size_t	s_len;

	(void)env;
	if (!s)
		return (NULL);
	i = 0;
	s_len = ft_gnl_strlen(s);
	if (start >= s_len)
		return (handle_empty_substr(env));
	if (len > s_len - start)
		len = s_len - start;
	new_s = allocate_substr(len, env);
	if (!new_s)
		return (NULL);
	while (i < len && s[start + i] != ':')
	{
		new_s[i] = s[start + i];
		i++;
	}
	new_s[i++] = '/';
	new_s[i] = '\0';
	return (new_s);
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
