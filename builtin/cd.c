/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bolcay <bolcay@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/23 09:01:29 by bolcay            #+#    #+#             */
/*   Updated: 2025/04/10 18:56:09 by bolcay           ###   ########.fr       */
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
	temp = ft_strdup(env->envp[j]);
	return (temp);
}

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
		env->envp[i] = ft_strjoin("PWD=", new_pwd);
		free(new_pwd);
	}
}

void	run_cd(char **args, t_env *env)
{
	char	*path;
	char	*old_pwd;
	char	*temp;

	temp = NULL;
	path = getenv("HOME");
	old_pwd = get_old_pwd(env);
	update_old_pwd(env);
	if (!args[1] || (args[1] && args[1][0] == '~'))
		env->exit_code = chdir(path);
	else if (args[1][0] == '-')
	{
		temp = ft_substr(old_pwd, 7, ft_strlen(old_pwd) - 7);
		env->exit_code = chdir(temp);
		printf("%s\n", temp);
		free(temp);
	}
	else
		env->exit_code = chdir(args[1]);
	if (env->exit_code == -1)
	{
		env->envp = remove_env(env->envp, "OLDPWD");
		env->envp = update_env(env->envp, old_pwd);
		DEBUG_PRINT(CYAN"old_pwd : %s\n"RESET, old_pwd);
		DEBUG_PRINT(GRN "exit status: %d\n" RESET, env->exit_code);
	}
	else
	{
		update_pwd(env);
		DEBUG_PRINT(GRN "exit status: %d\n" RESET, env->exit_code);
	}
	if (old_pwd)
		free(old_pwd);
}
