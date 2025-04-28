/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: batuhan <batuhan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/23 09:01:29 by bolcay            #+#    #+#             */
/*   Updated: 2025/04/22 14:48:24 by batuhan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"



int	no_path_handle(t_env *env)
{
	write(2, "minishell: cd: HOME not set\n", 28);
	env->exit_code = 1;
	return (1);
}

int	argument_check(char **args, t_env *env)
{
	int	i;

	i = 0;
	while (args[i])
		i++;
	if (i > 2)
	{
		write(2, "minishell: cd: too many arguments\n", 34);
		env->exit_code = 1;
		return (1);
	}
	return (0);
}

void	cd_minus_sign(char *args, char *path, t_env *env, char *old_pwd)
{
	char	*temp;

	if (args[1] && args[1] == '-')
	{
		env->exit_code = chdir(path);
	}
	else if (!old_pwd || ft_strncmp(old_pwd, "OLDPWD=", 7) != 0)
	{
		write(2, "minishell: cd: OLDPWD not set \n", 31);
		env->exit_code = 1;
		return ;
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

void	cd_tilde(char *path, t_env *env)
{
	if (!path)
	{
		write(2, "minishell: cd: HOME not set\n", 28);
		env->exit_code = 1;
		return ;
	}
	env->exit_code = chdir(path);
}

void	cd_helper(char *args, t_env *env, char *old_pwd)
{
	if (env->exit_code == -1)
	{
		env->exit_code = 1;
		write(2, "minishell: cd: ", 15);
		write(2, args, ft_strlen(args));
		write(2, ": No such file or directory\n", 28);
		env->envp = remove_env(env->envp, "OLDPWD", env);
		if (old_pwd != NULL)
			env->envp = update_env(env->envp, old_pwd, env);
	}
	else
	{
		update_pwd(env);
		env->exit_code = 0;
	}
}


char	*get_old_pwd(t_env *env)
{
	int		j;
	char	*temp;

	j = 0;
	while (env->envp && env->envp[j]
		&& ft_strncmp(env->envp[j], "OLDPWD", 6) != 0)
		j++;
	if (env->envp[j])
		temp = ft_strdup(env->envp[j]);
	else
		temp = NULL;
	return (temp);
}

void	uop_helper(t_env *env, int j)
{
	int		i;
	char	*temp;

	i = 0;
	while (env->envp[i])
	{
		if (ft_strncmp(env->envp[i], "PWD", 3) == 0)
		{
			temp = ft_substr(env->envp[i], 4, ft_strlen(env->envp[i]) - 4);
			env->envp[j] = ft_strjoin("OLDPWD=", temp);
			gc_register(env->gc, env->envp[j]);
			free(temp);
			return ;
		}
		i++;
	}
}

void	update_old_pwd(t_env *env, int check)
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
		env->envp = update_env(env->envp, temp, env);
		env->export = update_env(env->export, temp, env);
		free(temp);
		return ;
	}
	while (env->envp[j] && ft_strncmp(env->envp[j], "OLDPWD", 6) != 0)
		j++;
	uop_helper(env, j);
}

void	update_pwd(t_env *env)
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
		env->envp[i] = ft_strjoin("PWD=", new_pwd);
		gc_register(env->gc, env->envp[i]);
		free(new_pwd);
		return ;
	}
	free(new_pwd);
}

void	run_cd(char **args, t_env *env)
{
	char	*path;
	char	*old_pwd;
	int		check;

	check = argument_check(args, env);
	if (check != 0)
		return ;
	path = getenv("HOME");
	if (!path)
		check = no_path_handle(env);
	if (check == 1)
		return ;
	old_pwd = get_old_pwd(env);
	if (!old_pwd)
		check = 1;
	update_old_pwd(env, check);
	if (!args[1] || (args[1] && args[1][0] == '~'))
		cd_tilde(path, env);
	else if (args[1][0] == '-')
		cd_minus_sign(args[1], path, env, old_pwd);
	else
		env->exit_code = chdir(args[1]);
	cd_helper(args[1], env, old_pwd);
	if (old_pwd)
		free(old_pwd);
}

// char	*get_old_pwd(t_env *env)
// {
// 	int		j;
// 	char	*temp;
	
// 	j = 0;
// 	while (env->envp[j] && ft_strncmp(env->envp[j], "OLDPWD", 6) != 0)
// 		j++;
// 	if (env->envp[j])
// 		temp = ft_strdup(env->envp[j]);
// 	else
// 		temp = NULL;
// 	return (temp);
// }

// void	update_old_pwd(t_env *env, int check)
// {
// 	char	*temp;
// 	char	buf[BUFSIZ];
// 	int		i;
// 	int		j;

// 	i = 0;
// 	j = 0;
// 	if (check == 1)
// 	{
// 		if (!getcwd(buf, BUFSIZ))
// 		{
// 			perror("cd: getcwd fauiled");
// 			return ;
// 		}
// 		temp = ft_strjoin("OLDPWD=", buf);
// 		env->envp = update_env(env->envp, temp, env);
// 		env->export = update_env(env->export, temp, env);
// 		free(temp);
// 		return ;
// 	}
// 	while (env->envp[j] && ft_strncmp(env->envp[j], "OLDPWD", 6) != 0)
// 		j++;
// 	while (env->envp[i])
// 	{
// 		if (ft_strncmp(env->envp[i], "PWD", 3) == 0)
// 		{
// 			// free(env->envp[j]);
// 			temp = ft_substr(env->envp[i], 4, ft_strlen(env->envp[i]) - 4);
// 			env->envp[j] = ft_strjoin("OLDPWD=", temp);
// 			gc_register(env->gc, env->envp[j]);
// 			free(temp);
// 			return ;
// 		}
// 		i++;
// 	}
// }

// void	update_pwd(t_env *env)
// {
// 	char	*new_pwd;
// 	char	temp[BUFSIZ];
// 	int		i;

// 	i = 0;
// 	if (!getcwd(temp, BUFSIZ))
// 	{
// 		perror("cd: getcwd failed");
// 		return ;
// 	}
// 	new_pwd = ft_strdup(temp);
// 	while (env->envp[i] && ft_strncmp(env->envp[i], "PWD", 3) != 0)
// 		i++;
// 	if (env->envp[i])
// 	{
// 		// free(env->envp[i]);
// 		env->envp[i] = ft_strjoin("PWD=", new_pwd);
// 		gc_register(env->gc, env->envp[i]);
// 		free(new_pwd);
// 		return ;
// 	}
// 	free(new_pwd);
// }

// void	run_cd(char **args, t_env *env)
// {
// 	char	*path;
// 	char	*old_pwd;
// 	char	*temp;
// 	int		check;
// 	int		i;

// 	i = 0;
// 	while (args[i])
// 		i++;
// 	if (i > 2)
// 	{
// 		fprintf(stderr, "minishell: cd: too many arguments\n");
// 		env->exit_code = 1;
// 		return ;
// 	}
// 	temp = NULL;
// 	check = 0;
// 	path = getenv("HOME");
// 	if (!path)
// 	{
// 		fprintf(stderr, "minishell: cd: HOME not set\n");
// 		env->exit_code = 1;
// 		return ;
// 	}
// 	old_pwd = get_old_pwd(env);
// 	if (!old_pwd)
// 		check = 1;
// 	update_old_pwd(env, check);
// 	if (!args[1] || (args[1] && args[1][0] == '~'))
// 	{
// 		path = getenv("HOME");
// 		if (!path)
// 		{
// 			fprintf(stderr, "minishell: cd: HOME not set\n");
// 			env->exit_code = 1;
// 			if (old_pwd)
// 				free(old_pwd);
// 			return ;
// 		}
// 		env->exit_code = chdir(path);
// 	}
// 	else if (args[1][0] == '-')
// 	{
// 		if (args[1][1] && args[1][1] == '-')
// 		{
// 			env->exit_code = chdir(path);
// 		}
// 		else if (!old_pwd || ft_strncmp(old_pwd, "OLDPWD=", 7) != 0)
// 		{
// 			fprintf(stderr, "minishell: cd: OLDPWD not set\n");
// 			env->exit_code = 1;
// 			if(old_pwd)
// 				free(old_pwd);
// 			return;
// 		}
// 		else
// 		{
// 			temp = ft_substr(old_pwd, 7, ft_strlen(old_pwd) - 7);
// 			env->exit_code = chdir(temp);
// 			if (env->exit_code == 0)
// 				printf("%s\n", temp);
// 			free(temp);
// 		}
// 	}
// 	else
// 		env->exit_code = chdir(args[1]);
// 	if (env->exit_code == -1)
// 	{
// 		env->exit_code = 1;
// 		fprintf(stderr, "minishell: cd: %s: No such file or directory\n", args[1]);
// 		env->envp = remove_env(env->envp, "OLDPWD", env);
// 		if (old_pwd != NULL)
// 			env->envp = update_env(env->envp, old_pwd, env);
// 	}
// 	else
// 	{
// 		update_pwd(env);
// 		env->exit_code = 0;
// 	}
// 	if (old_pwd)
// 		free(old_pwd);
// 	// printf("%s\n", args[1]);
// }
