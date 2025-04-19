/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: batuhan <batuhan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/23 09:01:29 by bolcay            #+#    #+#             */
/*   Updated: 2025/04/19 14:42:34 by batuhan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static char	*get_old_pwd(t_env *env)
{
	int		j;
	char	*temp;
	
	j = 0;
	while (env->envp[j] && ft_strncmp(env->envp[j], "OLDPWD", 6) != 0)
		j++;
	if (env->envp[j])
		temp = ft_strdup(env->envp[j]);
	else
		temp = NULL;
	return (temp);
}

static void	update_old_pwd(t_env *env, int check)
{
	char	*temp;
	char	buf[BUFSIZ];
	int		i;
	int		j;

	i = 0;
	j = 0;
	if (check == 1)
	{
		if (!getcwd(buf, BUFSIZ))
		{
			perror("cd: getcwd fauiled");
			return ;
		}
		temp = ft_strjoin("OLDPWD=", buf);
		env->envp = update_env(env->envp, temp);
		env->export = update_env(env->export, temp);
		free(temp);
		return ;
	}
	while (env->envp[j] && ft_strncmp(env->envp[j], "OLDPWD", 6) != 0)
		j++;
	while (env->envp[i])
	{
		if (ft_strncmp(env->envp[i], "PWD", 3) == 0)
		{
			free(env->envp[j]);
			temp = ft_substr(env->envp[i], 4, ft_strlen(env->envp[i]) - 4);
			env->envp[j] = ft_strjoin("OLDPWD=", temp);
			free(temp);
			return ;
		}
		i++;
	}
}

static void	update_pwd(t_env *env)
{
	char	*new_pwd;
	char	temp[BUFSIZ];
	int		i;

	i = 0;
	if (!getcwd(temp, BUFSIZ))
	{
		perror("cd: getcwd failed");
		return ;
	}
	new_pwd = ft_strdup(temp);
	while (env->envp[i] && ft_strncmp(env->envp[i], "PWD", 3) != 0)
		i++;
	if (env->envp[i])
	{
		free(env->envp[i]);
		env->envp[i] = ft_strjoin("PWD=", new_pwd);
		free(new_pwd);
	}
}

void	run_cd(char **args, t_env *env)
{
	char	*path;
	char	*old_pwd;
	char	*temp;
	int		check;
	int		i;

	i = 0;
	while (args[i])
		i++;
	if (i > 2)
	{
		fprintf(stderr, "minishell: cd: too many arguments\n");
		env->exit_code = 1;
		return ;
	}
	temp = NULL;
	check = 0;
	path = getenv("HOME");
	if (!path)
	{
		fprintf(stderr, "minishell: cd: HOME not set\n");
		env->exit_code = 1;
		return ;
	}
	old_pwd = get_old_pwd(env);
	if (!old_pwd)
		check = 1;
	update_old_pwd(env, check);
	if (!args[1] || (args[1] && args[1][0] == '~'))
	{
		path = getenv("HOME");
		if (!path)
		{
			fprintf(stderr, "minishell: cd: HOME not set\n");
			env->exit_code = 1;
			if (old_pwd)
				free(old_pwd);
			return ;
		}
		env->exit_code = chdir(path);
	}
	else if (args[1][0] == '-')
	{
		if (!old_pwd || ft_strncmp(old_pwd, "OLDPWD=", 7) != 0)
		{
			fprintf(stderr, "minishell: cd: OLDPWD not set\n");
			env->exit_code = 1;
			if(old_pwd)
				free(old_pwd);
			return;
		}
		else
		{
			temp = ft_substr(old_pwd, 7, ft_strlen(old_pwd) - 7);
			env->exit_code = chdir(temp);
			if (env->exit_code == 0)
				printf("%s\n", temp);
			free(temp);
		}
	}
	else
		env->exit_code = chdir(args[1]);
	if (env->exit_code == -1)
	{
		env->exit_code = 1;
		fprintf(stderr, "minishell: cd: %s: No such file or directory\n", args[1]);
		env->envp = remove_env(env->envp, "OLDPWD");
		if (old_pwd != NULL)
			env->envp = update_env(env->envp, old_pwd);
	}
	else
	{
		update_pwd(env);
		env->exit_code = 0;
	}
	if (old_pwd)
		free(old_pwd);
	// printf("%s\n", args[1]);
}
