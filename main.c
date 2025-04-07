/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bolcay <bolcay@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 10:09:58 by bolcay            #+#    #+#             */
/*   Updated: 2025/03/25 12:36:27 by bolcay           ###   ########.fr       */
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

// void    initialized_env(t_env *env, char **envp)
// {
//     env->envp = envp;
//     env->exit_code = 0;
//     env->curr_pwd = getcwd(NULL, 0);
//     env->old_pwd = NULL;
//     env->path = NULL;
//     env->path1 = NULL;
//     env->export = NULL;

//     g_env = *env;
//     g_env.exit_code = 0;
//     g_env.curr_pwd = ft_strdup(env->curr_pwd);
//     g_env.old_pwd = NULL;
//     g_env.path = NULL;
//     g_env.path1 = NULL;
//     g_env.export = NULL;
//     DEBUG_PRINT(CYAN"Environment initialized\n"RESET);
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
    {
        line = readline(CYAN"minishell> "RESET);
        if(!line)
            return (NULL); // Control D and EOF situation
        if(*line)
            add_history(line);
        return line;
    }
    else
    {
        line = get_next_line(STDIN_FILENO);
        if(!line)
            return (NULL);
        trimmer = ft_strtrim(line, "\n");
        free(line);
    }
    return (trimmer);
}

int main(int ac, char **av, char **envp)
{
    char   *line;
    t_env   *env = NULL;
    t_token *tokens;
	(void)av;

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
        tokens = tokenizer(line, env);
        if(!tokens)
        {
            free(line);
            continue;
        }
        // initiate_env(env, envp);
        cell_launch(tokens, env); // a function that runs the programs in the computer
        free_token_matrix(tokens);
        free(line);
        signal_mode_command();
        // added_process(line, envp);
    }
    free(env);
	return (0);
}
