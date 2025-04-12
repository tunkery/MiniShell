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

static int count_token_args(t_token *tmp)
{
	int count;

	count = 0;
	while (tmp && tmp->type != TOKEN_SEMIC && tmp->type != TOKEN_REDIRECT_APPEND
		&& tmp->type != TOKEN_HEREDOC && tmp->type != TOKEN_REDIRECT_OUT
		&& tmp->type != TOKEN_REDIRECT_IN && tmp->type != TOKEN_HEREDOC
		&& tmp->type != TOKEN_PIPE)
	{
		if (tmp->type == TOKEN_WORD)
			count++;
		tmp = tmp->next;
	}
	return (count);
}

char	**tokens_to_args(t_token *tokens)
{
	t_token	*tmp;
	char	**args;
	int		i;

	tmp = tokens;
	i = 0;
	args = malloc((count_token_args(tokens) + 1) * sizeof(char *));
	if (!args)
		return (NULL);
	tmp = tokens;
	while (tmp && tmp->type != TOKEN_SEMIC && tmp->type != TOKEN_REDIRECT_APPEND
		&& tmp->type != TOKEN_HEREDOC && tmp->type != TOKEN_REDIRECT_OUT
		&& tmp->type != TOKEN_REDIRECT_IN && tmp->type != TOKEN_HEREDOC
		&& tmp->type != TOKEN_PIPE)
	{
		if (tmp->type == TOKEN_WORD)
			args[i++] = ft_strdup(tmp->value);
		tmp = tmp->next;
	}
	args[i] = NULL;
	return (args);
}

void	execute_with_redirection(char **args, t_env *env, int out_fd,
		int save_stdout)
{
	if (builtin_check(args) != 0)
	{
		if (out_fd != STDOUT_FILENO)
		{
			if (dup2(out_fd, STDOUT_FILENO) == -1)
				perror("dup2 failed!");
			close(out_fd);
			DEBUG_PRINT(GRN "Output redirected to file\n" RESET);
		}
		run_builtin(args, env);
		if (out_fd != STDOUT_FILENO)
		{
			if (dup2(save_stdout, STDOUT_FILENO) == -1)
				perror("dup2 failed!");
			DEBUG_PRINT(GRN "STDOUT restored\n" RESET);
		}
	}
	else
	{
		exec_command(args, env, out_fd);
		DEBUG_PRINT(GRN "Command executed\n" RESET);
	}
}

void	handle_redirection(t_token **current, char **args, int *out_fd,
		char **heredoc_input, t_env *env)
{
	int in_fd = STDIN_FILENO;
	while (*current && (*current)->type != TOKEN_SEMIC && (*current)->type != TOKEN_PIPE)
	{
		if ((*current)->type == TOKEN_REDIRECT_OUT) // >
		{
			*current = (*current)->next;
			openfile_redirected(current, out_fd, args, 0);
		}
		else if (*current && (*current)->type == TOKEN_REDIRECT_IN) // <
		{
			*current = (*current)->next;
			read_redirected_in(current, &in_fd, args, env);
			if(!*args)
				return ;
		}
		else if ((*current)->type == TOKEN_REDIRECT_APPEND) // >>
		{
			*current = (*current)->next;
			openfile_redirected(current, out_fd, args, 1);
		}
		else if ((*current)->type == TOKEN_HEREDOC) // <<
		{
			*current = (*current)->next;
			process_child_heredoc(current, heredoc_input, args, env);
		}
		else
			(*current) = (*current)->next;
	}
}



void	exec_without_pipes(t_token *tokens, t_env *env)
{
    t_token	*tmp;
    char	**args;
    int		out_fd;
    int		save_stdout;
    int		save_stdin;
    char	*heredoc_input;
    int		i;

    tmp = tokens;
    args = NULL;
    out_fd = STDOUT_FILENO;
    save_stdout = dup(STDOUT_FILENO);
    save_stdin = dup(STDIN_FILENO);
    heredoc_input = NULL;

    while (tmp)
    {
        args = tokens_to_args(tmp);
        if (!args)
        {
            DEBUG_PRINT(RED "Failed to convert tokens to args\n" RESET);
            while (tmp && tmp->type != TOKEN_SEMIC)
                tmp = tmp->next;
            if (tmp)
                tmp = tmp->next;
            continue;
        }
        
        DEBUG_PRINT(BLUE "Args created\n" RESET);
        i = 0;
        while (args[i])
        {
            DEBUG_PRINT(BLUE "Args[%d]: %s\n" RESET, i, args[i]);
            i++;
        }
        
        handle_redirection(&tmp, args, &out_fd, &heredoc_input, env);
        if(args && args[0])
        {
            execute_with_redirection(args, env, out_fd, save_stdout);
        }
        else
        {
            DEBUG_PRINT(RED"Skip execution due to redirection failed!"RESET);
        }
        
        // clean_2d(args);
        args = NULL;
        
        if (out_fd != STDOUT_FILENO)
            close(out_fd);
        out_fd = STDOUT_FILENO;
        
        if (dup2(save_stdin, STDIN_FILENO) == -1)
            perror("dup2 failed to restore STDIN");
        
        while (tmp && tmp->type != TOKEN_SEMIC)
        {
            DEBUG_PRINT(RED"Advancing tmp: %p, type: %d\n"RESET, tmp, tmp->type);
            tmp = tmp->next;
        }
        if (tmp)
        {
            DEBUG_PRINT(RED"Skipping semicol"RESET);
            tmp = tmp->next;
        }
    }
    
    close(save_stdout);
    close(save_stdin);
}

void	cell_launch(t_token *tokens, t_env *env)
{
    DEBUG_PRINT(BLUE "Starting Cell_lounch\n" RESET);

    if(has_pipes(tokens))
    {
        DEBUG_PRINT(BLUE "Pipe detected, using execute_piped_commands\n" RESET);
        execute_piped_command(tokens, env);
    }
    else
    {
        DEBUG_PRINT(BLUE "No pipes detected, using normal execution\n" RESET);
        exec_without_pipes(tokens, env);
    }

    DEBUG_PRINT(BLUE "Ending Cell_lounch\n" RESET);
}
