/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bolcay <bolcay@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/23 09:00:31 by bolcay            #+#    #+#             */
/*   Updated: 2025/04/02 14:44:46 by bolcay           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	run_unset(char **args, t_env *env)
{
	env->envp = remove_env(env->envp, args[1]);
	env->export = remove_env(env->export, args[1]);
	env->exit_code = 0;
	DEBUG_PRINT(GRN "exit status: %d\n" RESET, env->exit_code);
}
