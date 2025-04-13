/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bolcay <bolcay@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/23 09:00:31 by bolcay            #+#    #+#             */
/*   Updated: 2025/04/13 17:47:00 by bolcay           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static int	get_path(char *str, t_env *env)
{
	int		j;
	int		size;
	char	*key;

	j = 0;
	key = ft_substr(str, 0, key_size(str));
	size = ft_strlen(key);
	while (env->envp[j] && ft_strncmp(env->envp[j], key, size) != 0)
		j++;
	if (!env->envp[j])
		return (-1);
	return (0);
}

void	run_unset(char **args, t_env *env)
{
	int	i;

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
