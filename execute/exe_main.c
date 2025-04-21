/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exe_main.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bolcay <bolcay@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/24 12:04:28 by hpehliva          #+#    #+#             */
/*   Updated: 2025/04/17 15:36:32 by bolcay           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static int count_token_args(t_token *tmp)
{
	t_token *tokens;
	t_token *next = NULL;
	int total;

	tokens = tmp;
	total = 0;
	while (tokens && tokens->type != TOKEN_SEMIC && tokens->type != TOKEN_PIPE)
	{
		if(tokens == next)
		{
			next = NULL;
			tokens = tokens->next;
			continue;
		}
		if(tokens->type == TOKEN_REDIRECT_APPEND
			|| tokens->type == TOKEN_HEREDOC ||
			tokens->type == TOKEN_REDIRECT_OUT ||
			 tokens->type == TOKEN_REDIRECT_IN || 
			 tokens->type == TOKEN_HEREDOC)
			 {
				next = tokens->next;
				tokens = tokens->next;
				continue;
			 }
		if (tokens->type == TOKEN_WORD)
		{
			total++;
		}
		tokens = tokens->next;
	}
	return (total);
}

char	**tokens_to_args(t_token *tokens)
{
	t_token	*tmp;
	t_token *next = NULL;
	char	**args;
	int		i;
	int total;

	total = count_token_args(tokens);

	args = malloc((total + 1) * sizeof(char *));
	if (!args)
		return (NULL);
	tmp = tokens;
	i = 0;
	while (tmp && tmp->type != TOKEN_SEMIC && tmp->type != TOKEN_PIPE)
	{
		if(tmp == next)
		{
			next = NULL;
			tmp = tmp->next;
			continue;
		}
		if(tmp->type == TOKEN_REDIRECT_APPEND
			|| tmp->type == TOKEN_HEREDOC ||
			tmp->type == TOKEN_REDIRECT_OUT ||
			 tmp->type == TOKEN_REDIRECT_IN || 
			 tmp->type == TOKEN_HEREDOC)
			 {
				next = tmp->next;
				tmp = tmp->next;
				continue;
			 }
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
		}
		run_builtin(args, env);
		if (out_fd != STDOUT_FILENO)
		{
			if (dup2(save_stdout, STDOUT_FILENO) == -1)
				perror("dup2 failed!");
		}
	}
	else
	{
		exec_command(args, env, out_fd);
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
			if(!*args)
			{
				return ;
			}
			
		}
		else if (*current && (*current)->type == TOKEN_REDIRECT_IN) // <
		{
			*current = (*current)->next;
			read_redirected_in(current, &in_fd, args, env);
			if(!*args)
			{
				return ;
			}
		}
		else if ((*current)->type == TOKEN_REDIRECT_APPEND) // >>
		{
			*current = (*current)->next;
			openfile_redirected(current, out_fd, args, 1);
			if(!*args)
			{
				return ;
			}
			if(!*current)
				break;
		}
		else if ((*current)->type == TOKEN_HEREDOC) // <<
		{
			*current = (*current)->next;
			process_child_heredoc(current, heredoc_input, args, env);
			if(!*args)
			{
				return ;
			}
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
    // int		i;

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
            while (tmp && tmp->type != TOKEN_SEMIC)
                tmp = tmp->next;
            if (tmp)
                tmp = tmp->next;
            continue;
        }
        // i = 0;
        // while (args[i])
        // {
		// 	// DEBUG_PRINT(MGNT"ARGS[%d]: %s\n"RESET, i, args[i]);
        //     i++;
        // }
		if (out_fd != STDOUT_FILENO)
            close(out_fd);
        out_fd = STDOUT_FILENO;
        
		env->exit_code = 0;

        handle_redirection(&tmp, args, &out_fd, &heredoc_input, env);
        if(args && args[0] && args[0][0] != '\0')
        {
            execute_with_redirection(args, env, out_fd, save_stdout);
        }

		
        args = NULL;
        
        if (out_fd != STDOUT_FILENO)
            close(out_fd);
        out_fd = STDOUT_FILENO;
        
        if (dup2(save_stdin, STDIN_FILENO) == -1)
            perror("dup2 failed to restore STDIN");
        
        while (tmp && tmp->type != TOKEN_SEMIC)
        {
            tmp = tmp->next;
        }
        if (tmp)
        {
            tmp = tmp->next;
        }
    }
    
    close(save_stdout);
    close(save_stdin);
}

void	cell_launch(t_token *tokens, t_env *env)
{

	if(!validate_syntax(tokens, env))
	{
		return ;
	}

    if(has_pipes(tokens))
    {
        execute_piped_command(tokens, env);
    }
    else
    {
        exec_without_pipes(tokens, env);
    }

}
