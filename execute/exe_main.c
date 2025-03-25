/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exe_main.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bolcay <bolcay@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/24 12:04:28 by hpehliva          #+#    #+#             */
/*   Updated: 2025/03/25 13:25:58 by bolcay           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char **tokens_to_args(t_token *tokens)
{
    int count = 0;
    t_token *tmp = tokens;
    char **args;
    int i = 0;

	/* Only count till pipe or directions */
    // while(tmp && tmp->type != TOKEN_SEMIC)
    // {
    //     if (tmp->type == TOKEN_WORD)
    //         count++;
    //     tmp = tmp->next;
    // }
	while(tmp && tmp->type != TOKEN_SEMIC && tmp->type != TOKEN_REDIRECT_APPEND &&
		 tmp->type != TOKEN_HEREDOC && tmp->type != TOKEN_REDIRECT_OUT  &&
		 tmp->type != TOKEN_REDIRECT_IN && tmp->type != TOKEN_HEREDOC && tmp->type != TOKEN_PIPE)
	{
		if (tmp->type == TOKEN_WORD)
			count++;
		tmp = tmp->next;
	}

    args = malloc((count + 1) * sizeof(char *));
    if(!args)
        return NULL;
    tmp = tokens;
	while(tmp && tmp->type != TOKEN_SEMIC && tmp->type != TOKEN_REDIRECT_APPEND &&
		tmp->type != TOKEN_HEREDOC && tmp->type != TOKEN_REDIRECT_OUT  &&
		tmp->type != TOKEN_REDIRECT_IN && tmp->type != TOKEN_HEREDOC && tmp->type != TOKEN_PIPE)
    {
        if(tmp->type == TOKEN_WORD)
        {
            args[i] = strdup(tmp->value); // change strdup to ft_strdup and test it again //TODO
            i++;
        }
        tmp = tmp->next;
    }
    args[i] = NULL;
    return args;
}

// a function that runs the programs in the computer
/*
    NOTES:
    - The dup() system call creates a duplicate of the file descriptor oldfd, using the lowest-numbered unused file descriptor for the new descriptor.
    - The dup2() system call performs the same task as dup(), but instead of using the lowest-numbered unused file descriptor, it uses the file descriptor number specified in newfd.
    
*/


void	cell_launch(t_token *tokens, t_env *env)
{
	t_token *tmp = tokens;
	char **args = NULL;
	int out_fd = STDOUT_FILENO;
	int save_stdout = dup(STDOUT_FILENO);
	char *heredoc_input = NULL;
	t_token *current;

	DEBUG_PRINT(BLUE"Starting Cell_lounch\n"RESET);
	while(tmp)
	{
		args = tokens_to_args(tmp);
		if(!args)
		{
			DEBUG_PRINT(RED"Failed to convert tokens to args\n"RESET);
			while(tmp && tmp->type != TOKEN_SEMIC)
				tmp = tmp->next;
			if(tmp)
				tmp = tmp->next;
			continue;
		}
		DEBUG_PRINT(BLUE"Args created\n"RESET);
		/* Check it all aguments in here*/
		int i = 0;
		while (args[i])
		{
			DEBUG_PRINT(BLUE"Args[%d]: %s\n"RESET, i, args[i]);
			i++;
		}
		current = tmp;
		/*
			Sadece kontrol >> ve << icin varr!!
		*/
		while(current && current->type != TOKEN_SEMIC)
		{
			/* Sembolu eklemek */
			if(current->type == TOKEN_REDIRECT_OUT) // > 
			{
				current = current->next;
				if(current && current->type == TOKEN_WORD)
				{
					out_fd = open(current->value, O_WRONLY | O_TRUNC | O_CREAT, 0644); // Uzerine yazdiriyor.
					if(out_fd < 0)
					{
						perror("open failed!");
						clean_2d(args);
						return ;
					}
					DEBUG_PRINT(BLUE"Redirecting output to '%s'\n"RESET, current->value);
				}
			}
			else if (current->type == TOKEN_REDIRECT_APPEND) // >>
			{ 
				current = current->next;
				if(current && current->type == TOKEN_WORD)
				{
					out_fd = open(current->value, O_WRONLY | O_APPEND | O_CREAT, 0644); // 644 means you can read and write the file or directory and other users can only read it
					if(out_fd < 0)
					{
						perror("open failed!");
						clean_2d(args);
						return ;
					}
					DEBUG_PRINT(BLUE"Redirecting aoutput to '%s'\n"RESET, current->value);
				}
			}
			else if(current->type == TOKEN_HEREDOC) // <<
			{
				current = current->next;
				if(current && current->type == TOKEN_WORD)
				{
					heredoc_input = handler_heredoc(current->value);
					int pipe_fd[2];
					if(pipe(pipe_fd) == -1)
					{
						perror("pipe failed!");
						clean_2d(args);
						return ;
					} // Surecler arasi boruyu olusturur > alt surec icin
					write(pipe_fd[1], heredoc_input, ft_strlen(heredoc_input));
					close(pipe_fd[1]);
					dup2(pipe_fd[0], STDIN_FILENO);
					close(pipe_fd[0]);
					free(heredoc_input);
					DEBUG_PRINT(GRN"HEredoc process for delimiter '%s'\n"RESET, current->value);
				}
			}
			current = current->next;
		}
		if(builtin_check(args) != 0)
		{
			if(out_fd != STDOUT_FILENO)
			{
				if(dup2(out_fd, STDOUT_FILENO) == -1)
					perror("dup2 failed!");
				close(out_fd);
				DEBUG_PRINT(GRN"Output redirected to file\n"RESET);
			}
			run_builtin(args, env);
			if(out_fd != STDOUT_FILENO)
			{
				if(dup2(save_stdout, STDOUT_FILENO) == -1)
					perror("dup2 failed!");
				DEBUG_PRINT(GRN"STDOUT restored\n"RESET);
			}
		}
		else
		{
			exec_command(args, env, out_fd);
			DEBUG_PRINT(GRN"Command executed\n"RESET);
		}
		clean_2d(args);
		if(out_fd != STDOUT_FILENO)
			close(out_fd);
		out_fd = STDOUT_FILENO;
		while(tmp && tmp->type != TOKEN_SEMIC)
			tmp = tmp->next;
		if(tmp)
			tmp = tmp->next;
	}
	close(save_stdout);
	DEBUG_PRINT(BLUE"Ending Cell_lounch\n"RESET);
}

