/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   envp.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: batuhan <batuhan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 12:08:19 by bolcay            #+#    #+#             */
/*   Updated: 2025/03/24 21:11:25 by batuhan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	**update_env(char **envp, char *key)
{
	int size;
	char **new_env;
	
	size = env_size(envp) + 1;
	new_env = malloc(sizeof(char *) * size);
	if (!new_env)
		return (NULL);
	copy_env(envp, &new_env);
	new_env[size - 1] = ft_strdup(key);
	new_env[size] = NULL;
	DEBUG_PRINT(GRN"New env created with key: %s\n"RESET, key);
	// asko freeleme cunku hata veriyo
	DEBUG_PRINT(GRN"Updated env with key: %s\n"RESET, key);
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
		{
			free(envp[i]);
			i++;
		}
		if (envp[i])
		{
			new_env[j] = ft_strdup(envp[i]);
			i++;
			j++;
		}
	}
	new_env[j] = NULL;
	// asko freeleme cunku hata veriyo
	DEBUG_PRINT(GRN"Removed env with key: %s\n"RESET, key);
	return (new_env);
}

void	initiate_env(t_env *env, char **envp)
{
	copy_env(envp, &(env->envp));
	copy_env(envp, &(env->export));
	if (!env->envp)
		return ;
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

