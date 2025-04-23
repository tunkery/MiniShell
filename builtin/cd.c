/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bolcay <bolcay@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/23 09:01:29 by bolcay            #+#    #+#             */
/*   Updated: 2025/04/23 16:05:40 by bolcay           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

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
