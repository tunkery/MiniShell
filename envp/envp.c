/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   envp.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bolcay <bolcay@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 12:08:19 by bolcay            #+#    #+#             */
/*   Updated: 2025/03/19 15:00:14 by bolcay           ###   ########.fr       */
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

void	initiate_env(t_env *env)
{
	env->path = malloc(sizeof(char **) * 2);
	if (!env->path)
		return ;
	env->path1 = getenv("PATH");
	env->path[0] = ft_gnl_substr(env->path1, 5, ft_strlen(env->path1) - 5);
	env->path[1] = NULL;
	
}

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

char	*find_exec(char *command, char *path, int i, int j)
{
	char *temp;

	while(path[i])
	{
		while (path[i] != ':')
			i++;
		temp = ft_gnls_substr(path, j, i - 4);
		temp = ft_strjoin(temp, command);
		if (access(temp, X_OK) == 0)
			return (temp);
		free(temp);
		j = i + 1;
		i++;
	}
	return (temp);
}

// a function that runs the programs in the computer

void	cell_launch(char **args, t_env *env)
{
	pid_t	pid;
	int		status;
	char *exec_path;

	if (!args || !args[0])
		return ;
	exec_path = find_exec(args[0], env->path1, 0, 1);
	if (!exec_path)
	{
		printf("minishell: %s: command not found.\n", args[0]);
		return ;
	}
	pid = fork();
	if (pid == 0)
	{
		if (execve(exec_path, args, env->path) == -1)
		{
			perror("execvp Failed");
			free(exec_path);
			exit(127);
		}
	}
	else if (pid < 0)
		perror("fork Failed");
	else
		waitpid(pid, &status, 0);
	free(exec_path);
}
