/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: batuhan <batuhan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/23 09:00:31 by bolcay            #+#    #+#             */
/*   Updated: 2025/04/14 18:17:32 by batuhan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static int	valid_name(char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (ft_isalnum(str[i]) == 0 && str[i] != '_')
		return (-1);
		i++;
	}
	return (0);
}

static int	get_path(char *str, t_env *env)
{
	int		j;
	int		size;
	char	*key;

	j = 0;
	key = ft_substr(str, 0, key_size(str));
	size = ft_strlen(key);
	while (env->envp && env->envp[j])
	{
		if (ft_strncmp(env->envp[j], key, size) == 0)
		{
			free(key);
			return (0);
		}
		j++;
	}
	free(key);
	return (-1);
}

void	run_unset(char **args, t_env *env)
{
	int	i;

	if (!args[1])
	{
		env->exit_code = 0;
		return ;
	}
	if (args[1][0] == '-')
	{
	// 	perror("unset");
		env->exit_code = 2;
		return ;
	}
	if (valid_name(args[1]) != 0)
	{
		env->exit_code = 1;
		return ;
	}
	if (ft_strchr(args[1], ';') != 0)
	{
		env->exit_code = 127;
		return ;
	}
	if (get_path(args[1], env) == 0)
	{
		env->exit_code = 1;
		return ;
	}
	if (args[1] && args[1][0] == '=')
	{
		env->exit_code = 1;
		return ;
	}
	i = 1;
	while (args[i])
	{
		env->envp = remove_env(env->envp, args[i]);
		env->export = remove_env(env->export, args[i]);
		i++;
	}
	env->exit_code = 0;
	DEBUG_PRINT(GRN "exit status: %d\n" RESET, env->exit_code);
}
