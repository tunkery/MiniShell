/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bolcay <bolcay@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 10:09:58 by bolcay            #+#    #+#             */
/*   Updated: 2025/03/18 17:07:56 by bolcay           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

//showing branch
/*
    Standart file descriptors:
        #define STDIN_FILENO    0        Standard input. 
        #define STDOUT_FILENO   1        Standard output.  
        #define STDERR_FILENO   2        Standard error output.
    
    - hpehliva$ hello
    bash: hello: command not found // Add this command!
*/
char    *user_input(void)
{
    char    *line;
    char *cpy;
    
    if(isatty(STDIN_FILENO)){
        return(readline(CYAN"minishell> "RESET));
    }
    line = get_next_line(STDIN_FILENO);
    if(!line)
        return (NULL); // Control D and EOF situation
    if(line)
    {
        cpy = line;
        line = ft_strtrim(line, "\n");
        free(cpy);
    }
    return (line);
}

int main(int ac, char **av, char **envp)
{
    char   *line;
	(void)av;
	(void)envp;

	if (ac != 1)
	{
		printf("Don't give any arguments!\n");
		return (0);
	}
    while(1)
    {
        signal_mode_read();
        line = user_input();
        if(!line)
            break;
        signal_mode_command();
        added_process(line, envp);
        free(line);
    }
	return (0);
}
