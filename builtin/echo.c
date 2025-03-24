/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: batuhan <batuhan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/23 08:56:22 by bolcay            #+#    #+#             */
/*   Updated: 2025/03/24 21:05:27 by batuhan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
/*
	This function will work only for flags!!
*/

// artik duzgun calisiyooooo!!!

void	run_echo(char **args, char *line)
{
	int	check;
	int	i;
	// int first;
	char *new;

	DEBUG_PRINT(BLUE"Running echo\n"RESET);
	check = 0;
	i = 1;
	// first = 1;
	if (!args[1]) // If there is no argumant just print it new line!
	{
		printf("\n");
		return ;
	}
	if (ft_strncmp(args[1], "-n", ft_strlen(args[1])) == 0 && ft_strlen(args[1]) == 2)
	{
		check = 1;
		i++;
		DEBUG_PRINT(GRN"Check is 1: Flag detected\n"RESET);
	}
	new = echo_separate(line, check);
	printf("%s", new);
	// while (args[i])
	// {
	// 	if(!first)
	// 		printf(" "); // Add space after first argument
	// 	printf("%s ", args[i]); //  Print argument only once.
	// 	first = 0;
	// 	i++;
	// }
	if (check == 0)
		printf("\n");
	DEBUG_PRINT(GRN"Echo completed\n"RESET);
}

// echo ve -n varsa onlari kaldirip onlarsiz olan stringi veriyor

char *echo_separate(char *line, int check)
{
	int i;
	char *temp;

	i = 6;
	if (!line)
		return (NULL);
	if (check > 0)
		i = 8;
	temp = ft_substr(line, i, ft_strlen(line) - i);
	return (temp);
}
