/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bolcay <bolcay@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/23 08:49:26 by bolcay            #+#    #+#             */
/*   Updated: 2025/03/23 09:11:00 by bolcay           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	run_env(char **args, t_env *env)
{
	int	i;

	i = 0;
	if (args[1])
	{
		printf("env: illegal option %s\n", args[1]);
		printf("usage: env\n");
		return ;
	}
	while (env->envp[i])
	{
		printf("%s\n", env->envp[i]);
		i++;
	}
}
