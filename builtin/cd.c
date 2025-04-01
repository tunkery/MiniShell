/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bolcay <bolcay@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/23 09:01:29 by bolcay            #+#    #+#             */
/*   Updated: 2025/04/01 11:35:59 by bolcay           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static void	update_old_pwd(t_env *env)
{
	char	*temp;
	int		i;
	int		j;

	i = 0;
	j = 0;
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
	char	*add_pwd;
	int		i;

	i = 0;
	if (!getcwd(temp, BUFSIZ))
		return ;
	new_pwd = ft_strdup(temp);
	while (env->envp[i] && ft_strncmp(env->envp[i], "PWD", 3) != 0)
		i++;
	if (env->envp[i])
	{
		free(env->envp[i]);
		add_pwd = ft_strdup("PWD=");
	}
}

void	run_cd(char **args, t_env *env)
{
	update_old_pwd(env);
	if (!args[1])
		return ;
	if (chdir(args[1]) != 0)
		return ;
	update_pwd(env);
}
