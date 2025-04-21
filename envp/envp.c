/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   envp.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bolcay <bolcay@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 12:08:19 by bolcay            #+#    #+#             */
/*   Updated: 2025/04/18 15:16:55 by bolcay           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	**update_env(char **envp, char *key)
{
	int		size;
	char	**new_env;
	int		i;
	
	if (!envp || !key)
		return (NULL);
	i = 0;
	size = env_size(envp);
	new_env = NULL;
	new_env = malloc(sizeof(char *) * (size + 2));
	if (!new_env)
		return (NULL);
	while (envp[i])
	{
		new_env[i] = ft_strdup(envp[i]);
		i++;
	}
	// copy_env(envp, &new_env);
	new_env[size] = ft_strdup(key);
	new_env[size + 1] = NULL;
	return (new_env);
}

char	**update_ex(char **envp, char *key)
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
	new_env = malloc(sizeof(char *) * (size + 2));
	if (!new_env)
		return (NULL);
	while (envp[i])
	{
		new_env[i] = ft_strdup(envp[i]);
		i++;
	}
	temp = copy_ex_helper(key);
	new_env[size] = ft_strdup(temp);
	free(temp);
	new_env[size + 1] = NULL;
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
	size = env_size(envp);
	new_env = malloc(sizeof(char *) * size);
	if (!new_env)
		return (NULL);
	while (envp[i])
	{
		if (ft_strncmp(envp[i], key, ft_strlen(key)) == 0
			&& (envp[i][ft_strlen(key)] == '=' || envp[i][ft_strlen(key)] == '\0'))
			free(envp[i++]);
		if (envp[i])
		{
			new_env[j] = ft_strdup(envp[i]);
			i++;
			j++;
		}
	}
	new_env[j] = NULL;
	return (new_env);
}

static void	shell_level_ex(char ***str, int i)
{
	char	*value;
	char	*temp;
	char	*temp2;
	int		digit;

	while ((*str)[i] && ft_strncmp((*str)[i], "SHLVL", 5) != 0)
		i++;
	if ((*str)[i][8])
		value = ft_substr((*str)[i], 6, 3);
	else if ((*str)[i][7])
		value = ft_substr((*str)[i], 6, 2);
	else
		value = ft_substr((*str)[i], 6, 1);
	digit = ft_atoi(value);
	free((*str)[i]);
	digit++;
	if (value)
		free(value);
	value = ft_itoa(digit);
	temp = ft_strjoin("SHLVL=", value);
	temp2 = copy_ex_helper(temp);
	free(temp);
	if (value)
		free(value);
	(*str)[i] = ft_strdup(temp2);
	if (temp2)
		free(temp2);
}

static void	shell_level(char ***str, int i)
{
	char	*value;
	char	*temp;
	int		digit;

	while ((*str)[i] && ft_strncmp((*str)[i], "SHLVL", 5) != 0)
		i++;
	if ((*str)[i][8])
		value = ft_substr((*str)[i], 6, 3);
	else if ((*str)[i][7])
		value = ft_substr((*str)[i], 6, 2);
	else
		value = ft_substr((*str)[i], 6, 1);
	digit = ft_atoi(value);
	free((*str)[i]);
	digit++;
	if (value)
		free(value);
	value = ft_itoa(digit);
	temp = ft_strjoin("SHLVL=", value);
	if (value)
		free(value);
	(*str)[i] = ft_strdup(temp);
	if (temp)
		free(temp);
}

void	initiate_env(t_env *env, char **envp)
{
	int	i;

	i = 0;
	copy_env(envp, &(env->envp));
	copy_ex(envp, &(env->export));
	shell_level(&(env->envp), 0);
	shell_level_ex(&(env->export), 0);
	env->exit_code = 0;
	env->save_stdin = -1;
	while (env->envp[i] && ft_strncmp(env->envp[i], "PWD", 3) != 0)
		i++;
	env->curr_pwd = ft_strdup(env->envp[i]);
	i = 0;
	while (env->envp[i] && ft_strncmp(env->envp[i], "OLDPWD", 6) != 0)
		i++;
	if (env->envp[i])
	{
		env->old_pwd = ft_strdup(env->envp[i]);
		if (!env->envp)
			return ;
	}
	env->path = malloc(sizeof(char **) * 2);
	if (!env->path)
		return ;
	env->path1 = getenv("PATH");
}

// helper function that's needed for the find_exec

static char	*ft_gnls_substr(char const *s, unsigned int start, size_t len)
{
	char	*new_s;
	size_t	i;
	size_t	s_len;

	if (!s)
		return (NULL);
	i = 0;
	s_len = ft_gnl_strlen(s);
	if (start >= s_len)
		return (ft_gnl_strdup(""));
	if (len > s_len - start)
		len = s_len - start;
	new_s = (char *)malloc(len + 1);
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

// finds the path to the executable we want to run
/*
	Execution part start in here!!
*/

char	*find_exec(char *command, char *path, int i, int j)
{
	char *temp;

	if (!command || !path)
		return (NULL);
	while(path[i])
	{
		while (path[i] && path[i] && path[i] != ':')
			i++;
		temp = ft_gnls_substr(path, j, i - 4);
		if(!temp)
			return (NULL);
		temp = ft_strjoin(temp, command);
		if (access(temp, X_OK) == 0)
			return (temp);
		free(temp);
		temp = NULL;
		if(!path[i])
			break;
		j = i + 1;
		i++;
	}
	return (temp);
}

