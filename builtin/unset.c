/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: batuhan <batuhan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/23 09:00:31 by bolcay            #+#    #+#             */
/*   Updated: 2025/04/12 15:04:08 by batuhan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	run_unset(char **args, t_env *env)
{
	int	i;

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
