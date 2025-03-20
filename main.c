/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bolcay <bolcay@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 10:09:58 by bolcay            #+#    #+#             */
/*   Updated: 2025/03/20 15:35:19 by bolcay           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// void added_process(char *line, char **envp)
// {
//     DEBUG_PRINT(RED"Proccesing line : %s \n", line);
//     t_token *tokens;
//     t_token *tmp;
//     (void)envp;
//     // t_process *process;

//     tokens = tokenizer(line);
//     if(!tokens)
//     {
//         DEBUG_PRINT(RED"No token to process\n"RESET);
//         return ;
//     }
//     tmp = tokens;
//     while(tmp)
//     {
//         DEBUG_PRINT(RED"Token: type= %d, value = '%s' \n", tmp->type, tmp->value);
//         tmp = tmp->next;
//     }

//     free_token_matrix(tokens);
//     DEBUG_PRINT(RED"Line processing completed\n"RESET);
// }



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
    char *trimmer;
    

    if(isatty(STDIN_FILENO))
        return(readline(CYAN"minishell> "RESET));

    line = readline(CYAN"minishell> "RESET);
    if(isatty(STDIN_FILENO)) // If it is terminal
    {
        if(!line)
            return (NULL); // Control D and EOF situation
        if(!*line)
            add_history(line);
        return line;
    }
    line = get_next_line(STDIN_FILENO);
    if(!line)
        return (NULL);
    trimmer = ft_strtrim(line, "\n");
    free(line);

    return (trimmer);
}

// just a temporary function for word separation in the arguments

// static char **split_input(char *line)
// {
//     int i = 0;
//     char **args;
//     char *token;
//     int count = 0;
//     char *temp = strdup(line); // Copy to count tokens

//     if (!temp)
//         return NULL;

//     // Count words
//     token = strtok(temp, " ");
//     while (token)
//     {
//         count++;
//         token = strtok(NULL, " ");
//     }
//     free(temp);

//     // Allocate array (+1 for NULL)
//     args = malloc((count + 1) * sizeof(char *));
//     if (!args)
//         return NULL;

//     // Fill array
//     token = strtok(line, " ");
//     while (token)
//     {
//         args[i++] = strdup(token);
//         token = strtok(NULL, " ");
//     }
//     args[i] = NULL;
//     return args;
// }

static char **tokens_to_args(t_token *tokens)
{
    int count = 0;
    t_token *tmp = tokens;
    char **args;
    int i = 0;

    while(tmp)
    {
        if (tmp->type == TOKEN_WORD)
            count++;
        tmp = tmp->next;
    }

    args = malloc((count + 1) * sizeof(char *));
    if(!args)
        return NULL;
    tmp = tokens;
    while(tmp)
    {
        if(tmp->type == TOKEN_WORD)
        {
            args[i] = strdup(tmp->value);
            i++;
        }
        tmp = tmp->next;
    }
    args[i] = NULL;
    return args;
}

int main(int ac, char **av, char **envp)
{
    char   *line;
    t_env   *env = NULL;
    t_token *tokens;
    char **args;
    // char **args;
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
    initiate_env(env, envp);
    while(1)
    {
        signal_mode_read();
        line = user_input();
        if(!line)
            break; // We can add free(line) here. or each links free it.
        tokens = tokenizer(line);
        if(!tokens)
        {
            free(line);
            continue;
        }
        args = tokens_to_args(tokens);
        if (builtin_check(args) != 0)
            run_builtin(args, env);
        else if (args && args[0])
            cell_launch(args, env); // a function that runs the programs in the computer
        free_token_matrix(tokens);
        clean_2d(args);
        free(line);
        signal_mode_command();
        // added_process(line, envp);
    }
    free(env);
	return (0);
}
