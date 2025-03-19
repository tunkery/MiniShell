/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bolcay <bolcay@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 10:09:58 by bolcay            #+#    #+#             */
/*   Updated: 2025/03/19 15:04:36 by bolcay           ###   ########.fr       */
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
    
    line = readline(CYAN"minishell> "RESET);
    if(isatty(STDIN_FILENO)) // If it is terminal
    {
        if (line && *line)
        {
            add_history(line);
            // printf("%s\n", line);
            free(line);
        }
    }
    else // or not!
    {
        if(line){
            // printf("%s\n", line);
            free(line);
        }
    }
    return (line);
}

// just a temporary function for word separation in the arguments

static char **split_input(char *line)
{
    int i = 0;
    char **args;
    char *token;
    int count = 0;
    char *temp = strdup(line); // Copy to count tokens

    if (!temp)
        return NULL;

    // Count words
    token = strtok(temp, " ");
    while (token)
    {
        count++;
        token = strtok(NULL, " ");
    }
    free(temp);

    // Allocate array (+1 for NULL)
    args = malloc((count + 1) * sizeof(char *));
    if (!args)
        return NULL;

    // Fill array
    token = strtok(line, " ");
    while (token)
    {
        args[i++] = strdup(token);
        token = strtok(NULL, " ");
    }
    args[i] = NULL;
    return args;
}

int main(int ac, char **av, char **envp)
{
    char   *line;
    t_env   *env = NULL;
    char **args;
	(void)av;
	(void)envp;

	if (ac != 1)
	{
		printf("Don't give any arguments!\n");
		return (0);
	}
    env = malloc(sizeof(t_env));
    if (!env)
        return (0);
    initiate_env(env);
    while(1)
    {
        signal_mode_read();
        line = user_input();
        if(!line)
            break; // We can add free(line) here. or each links free it.
        args = split_input(line);
        if (args && args[0])
            cell_launch(args, env); // a function that runs the programs in the computer
        // free(line);
        // clean_2d(args);
        signal_mode_command();
        // process_input(line);
    }

	return (0);
}
