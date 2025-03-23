/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bolcay <bolcay@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/23 08:56:22 by bolcay            #+#    #+#             */
/*   Updated: 2025/03/23 13:46:05 by bolcay           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	run_echo(char **args)
{
	int	check;
	int	i;
	int first;

	DEBUG_PRINT(BLUE"Running echo\n"RESET);
	check = 0;
	i = 1;
	first = 1;
	if (!args[1])
	{
		printf("\n");
		return ;
	}
	// else if (ft_strncmp(args[1], "-n", ft_strlen(args[1])) == 0
	// 		&& !args[2])
	// 		return ;
	if (ft_strncmp(args[1], "-n", ft_strlen(args[1])) == 0)
	{
		check = 1;
		DEBUG_PRINT(GRN"Check is 1: Flag detected\n"RESET);
	}
		// check++;
	while (args[i])
	{
		if(i == 1 && ft_strncmp(args[i], "-n", ft_strlen(args[i])) == 0)
		{
			i++;
<<<<<<< HEAD
			continue;
		}
		// if (ft_strncmp(args[i], "-n", ft_strlen(args[i])) == 0
		// && i < 3)
		// 	i++;
		if(!first)
			printf(" ");
		printf("%s ", args[i]);
		first = 0;
=======
		if (args[i + 1])
			printf("%s ", args[i]);
		else
			printf("%s", args[i]);
>>>>>>> e8ce99ea3911f3eeccfacc55e03bc693ce906ab7
		i++;
	}
	if (check == 0)
		printf("\n");
	DEBUG_PRINT(GRN"Echo completed\n"RESET);
}
