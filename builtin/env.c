/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: batuhan <batuhan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/23 08:49:26 by bolcay            #+#    #+#             */
/*   Updated: 2025/04/14 17:57:42 by batuhan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	run_env(char **args, t_env *env)
{
	int	i;

	i = 0;
	if (args[1] && args[1][0] == '-')
	{
		fprintf(stderr, "env: illegal option %s\n", args[1]);
		printf("usage: env\n");
		env->exit_code = 1;
		DEBUG_PRINT(GRN "exit status: %d\n" RESET, env->exit_code);
		return ;
	}
	else if (args[1])
	{
		fprintf(stderr, "env: %s: No such file or directory\n", args[1]);
		env->exit_code = 127;
		DEBUG_PRINT(GRN "exit status: %d\n" RESET, env->exit_code);
		return ;
	}
	else
		env->exit_code = 0;
	while (env->envp[i])
	{
		fprintf(stdout, "%s\n", env->envp[i]);
		i++;
	}
	DEBUG_PRINT(GRN "exit status: %d\n" RESET, env->exit_code);
}
