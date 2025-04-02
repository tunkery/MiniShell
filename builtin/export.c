/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bolcay <bolcay@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/23 08:59:18 by bolcay            #+#    #+#             */
/*   Updated: 2025/04/02 14:54:36 by bolcay           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	run_export(char **args, t_env *env)
{
	int	i;

	i = 0;
	if (!args[1])
	{
		while (env->export[i])
		{
			printf("%s\n", env->export[i]);
			i++;
		}
		env->exit_code = 0;
		DEBUG_PRINT(GRN "exit status: %d\n" RESET, env->exit_code);
	}
	else if (ft_strchr(args[1], '=') != 0)
	{
		env->envp = update_env(env->envp, args[1]);
		env->export = update_env(env->export, args[1]);
		env->exit_code = 0;
		DEBUG_PRINT(GRN "exit status: %d\n" RESET, env->exit_code);
	}
	else
	{
		env->export = update_env(env->export, args[1]);
		env->exit_code = 0;
		DEBUG_PRINT(GRN "exit status: %d\n" RESET, env->exit_code);
	}
}
