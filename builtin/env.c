/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bolcay <bolcay@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/23 08:49:26 by bolcay            #+#    #+#             */
/*   Updated: 2025/04/17 15:29:15 by bolcay           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static void	env_helper(char *args, t_env *env)
{
	write(2, "env: illegal option ", 20);
	write(2, args, ft_strlen(args));
	write(2, "\n", 1);
	write(2, "usage: env\n", 11);
	env->exit_code = 1;
}

void	run_env(char **args, t_env *env)
{
	int	i;

	i = 0;
	if (args[1] && args[1][0] == '-')
	{
		env_helper(args[1], env);
		return ;
	}
	else if (args[1])
	{
		write(2, "env: ", 5);
		write(2, args[1], ft_strlen(args[1]));
		write(2, ": No such file or directory\n", 28);
		env->exit_code = 127;
		return ;
	}
	else
		env->exit_code = 0;
	while (env->envp[i])
	{
		printf("%s\n", env->envp[i]);
		i++;
	}
}
