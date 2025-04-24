/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd_helper.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bolcay <bolcay@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/23 16:02:38 by bolcay            #+#    #+#             */
/*   Updated: 2025/04/24 20:23:51 by bolcay           ###   ########.fr       */
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
		env->envp = remove_env(env->envp, "OLDPWD", env, 0);
		if (old_pwd != NULL)
			env->envp = update_env(env->envp, old_pwd, env);
	}
	else
	{
		update_pwd(env);
		env->exit_code = 0;
	}
}
