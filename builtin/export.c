/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bolcay <bolcay@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/23 08:59:18 by bolcay            #+#    #+#             */
/*   Updated: 2025/04/10 18:30:20 by bolcay           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static int	export_check(char **args)
{
	if (ft_strchr(args[1], '-') != 0)
		return (-1);
	if (ft_strchr(args[1], ' ') != 0)
		return (-1);
	if (ft_strchr(args[1], 9) != 0)
		return (-1);
	if (ft_strchr(args[1], 10) != 0)
		return (-1);
	if (ft_strchr(args[1], 11) != 0)
		return (-1);
	if (ft_strchr(args[1], 12) != 0)
		return (-1);
	if (ft_strchr(args[1], 13) != 0)
		return (-1);
	if (ft_isdigit(args[1][0]) != 0)
		return (-1);
	return (0);
}

void	run_export(char **args, t_env *env)
{
	int	i;

	i = 0;

	if (!args[1])
	{
		while (env->export[i])
			printf("%s\n", env->export[i++]);
		env->exit_code = 0;
	}
	else if (export_check(args) != 0)
		printf("minishell: export: '%s': not a valid identifier\n", args[1]);
	else if (ft_strchr(args[1], '=') != 0)
	{
		env->envp = update_env(env->envp, args[1]);
		env->export = update_env(env->export, args[1]);
		env->exit_code = 0;
	}
	else
	{
		env->export = update_env(env->export, args[1]);
		env->exit_code = 0;
	}
	DEBUG_PRINT(GRN "exit status: %d\n" RESET, env->exit_code);
}
