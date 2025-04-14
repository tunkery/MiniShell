/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: batuhan <batuhan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/23 08:53:58 by bolcay            #+#    #+#             */
/*   Updated: 2025/04/14 16:59:31 by batuhan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	run_pwd(char **args, t_env *env)
{
	char buf[BUFSIZ];
	char *check;
	
	if (args[1] && args[1][0] && args[1][0] == '-')
	{
		fprintf(stderr, "minishell: pwd: %s: invalid option\n", args[1]);
		printf("pwd: usage: pwd\n");
		env->exit_code = 1;
		DEBUG_PRINT(GRN "exit status: %d\n" RESET, env->exit_code);
		return ;
	}
	check = getcwd(buf, BUFSIZ);
	if (!check)
	{
		perror("minishell: pwd");
		env->exit_code = 1;
		return ;
	}
	printf("%s\n", buf);
	env->exit_code = 0;
	DEBUG_PRINT(GRN "exit status: %d\n" RESET, env->exit_code);
}
