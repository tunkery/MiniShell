/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bolcay <bolcay@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/23 08:53:58 by bolcay            #+#    #+#             */
/*   Updated: 2025/03/23 09:05:47 by bolcay           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	run_pwd(char **args)
{
	char buf[BUFSIZ];
	
	if (args[1] && args[1][0] == '-')
	{
		printf("minishell: pwd: %s: invalid option\n", args[1]);
		printf("pwd: usage: pwd\n");
		return ;
	}
	getcwd(buf, BUFSIZ);
	printf("%s\n", buf);
}
