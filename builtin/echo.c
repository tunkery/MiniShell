/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: batuhan <batuhan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/23 08:56:22 by bolcay            #+#    #+#             */
/*   Updated: 2025/04/15 17:03:02 by batuhan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
/*
	This function will work only for flags!!
*/

// artik duzgun calisiyooooo!!!
/*
 * Check if the string is a valid -n flag
 * A valid -n flag is "-n" or "-nnn..." (one or more 'n's after the dash)
 */
static int is_n_flag(char *str)
{
    if (!str || str[0] != '-')
        return (0);
    
    int i = 1;
    while (str[i])
    {
        if (str[i] != 'n')
            return (0);
        i++;
    }
    
    return (i > 1); // At least one 'n' after the dash
}

/*
 * Handle the special case of $? (exit status)
 */
// static int handle_exit_status(char **args, t_env *env)
// {
//     if (args[1] && args[1][0] == '$' && args[1][1] == '?' && !args[1][2])
//     {
//         printf("%d\n", env->exit_code);
//         env->exit_code = 0;
//         return (1);
//     }
//     return (0);
// }

void run_echo(char **args, t_env *env)
{
    int check;
    int i;

    
    // Check for $? (exit status)
    if (args[1] && args[1][0] == '$' && args[1][1] == '?' && !args[1][2])
    {
        printf("%d\n", env->exit_code);
        env->exit_code = 0;
        return;
    }
            
    // If no arguments, just print newline
    if (!args[1])
    {
        printf("\n");
        env->exit_code = 0;
        return;
    }
    
    // Check for -n flag(s)
    check = 0;
    i = 1;
    while (args[i] && is_n_flag(args[i]))
    {
        check = 1;
        i++;
    }
    
    // Print arguments with spaces between them
    int first = 1;
    while (args[i])
    {
        if (!first)
            printf(" ");
        printf("%s", args[i]);
        first = 0;
        i++;
    }
    
    // Add newline if -n flag wasn't provided
    if (check == 0)
        printf("\n");
    
    env->exit_code = 0;
}

	// int	check;
	// int	i;
	// int first;
	// // char *new;

	// DEBUG_PRINT(BLUE"Running echo\n"RESET);
	// check = 0;
	// i = 1;
	// // first = 1;
	// if (args[1] && args[1][0] == '$' && args[1][1] == '?')
	// {
	// 	printf("%d\n", env->exit_code);
	// 	env->exit_code = 0;
	// 	return ;
	// }
	// if (!args[1]) // If there is no argumant just print it new line!
	// {
	// 	printf("\n");
	// 	env->exit_code = 0;
	// 	DEBUG_PRINT(GRN "exit status: %d\n" RESET, env->exit_code);
	// 	return ;
	// }
	// if (ft_strncmp(args[1], "-n", ft_strlen(args[1])) == 0 && ft_strlen(args[1]) == 2)
	// {
	// 	check = 1;
	// 	i++;
	// 	DEBUG_PRINT(GRN"Check is 1: Flag detected\n"RESET);
	// }
	// // new = echo_separate(line, check);
	// // printf("%s", new);
	// while (args[i])
	// {
	// 	if(!first)
	// 		printf(" "); // Add space after first argument
	// 	printf("%s ", args[i]); //  Print argument only once.
	// 	first = 0;
	// 	i++;
	// }
	// if (check == 0)
	// 	printf("\n");
	// env->exit_code = 0;
	// DEBUG_PRINT(GRN "exit status: %d\n" RESET, env->exit_code);
	// DEBUG_PRINT(GRN"Echo completed\n"RESET);


// echo ve -n varsa onlari kaldirip onlarsiz olan stringi veriyor

// char *echo_separate(char *line, int check)
// {
// 	int i;
// 	char *temp;

// 	i = 5;
// 	if (!line)
// 		return (NULL);
// 	if (check > 0)
// 		i = 7;
// 	while ((line[i] >= 9 && line[i] <= 13) || (line[i] == 32))
// 		i++;
// 	if (line[i] == '"' && line[ft_strlen(line) - 1] == '"')
// 	{
// 		DEBUG_PRINT(BLUE"We're in the if statement now for the quotes\n"RESET);
// 		i++;
// 		temp = ft_substr(line, i, ft_strlen(line) - i - 1);
// 	}
// 	else
// 		temp = ft_substr(line, i, ft_strlen(line) - i);
// 	return (temp);
// }
