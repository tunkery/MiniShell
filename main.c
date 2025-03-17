/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bolcay <bolcay@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 10:09:58 by bolcay            #+#    #+#             */
/*   Updated: 2025/03/17 12:20:47 by bolcay           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void    read_line(void)
{
    char    *line;

    line = readline(CYAN"minishell> "RESET);
    if (line && *line)
    {
        add_history(line);
        printf("%s\n", line);
        free(line);
    }
    rl_clear_history();
}

int main(int ac, char **av, char **envp)
{

	(void)av;
	(void)envp;

	if (ac != 1)
	{
		printf("Don't give any arguments!\n");
		return (0);
	}
    read_line();

	return (0);
}
