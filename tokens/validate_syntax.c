/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   validate_syntax.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hpehliva <hpehliva@student.42heilbronn.de  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/13 18:38:08 by hpehliva          #+#    #+#             */
/*   Updated: 2025/04/13 18:38:10 by hpehliva         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"


void print_syntax_message(char *str, t_env *env)
{
    write(2, "minishell: syntax error near unexpected token `", 47);
    write(2, str, ft_strlen(str));
    write(2, " \n", 2);
    env->exit_code = 258;
}

int validate_redirect_syntax(t_token *tokens, t_env *env)
{
    t_token *tmp = tokens;

    while(tmp)
    {
        if(tmp->type == TOKEN_REDIRECT_IN || tmp->type == TOKEN_REDIRECT_OUT || 
            tmp->type == TOKEN_REDIRECT_APPEND || tmp->type == TOKEN_HEREDOC)
        {
            if(!tmp->next)
            {
                
                print_syntax_message("newline'", env);
                return 0;
            }
            if(tmp->next->type != TOKEN_WORD)
            {
                print_syntax_message(tmp->next->value, env);
                return 0;
            }
        }
        tmp = tmp->next;
    }
    return 1;
}

int validate_pipe_syntax(t_token *tokens, t_env *env)
{
    t_token *tmp = tokens;

    // For only one pipe!
    if(tmp && tmp->type == TOKEN_PIPE)
    {
        print_syntax_message("|'", env);
        return 0;
    }
    /*
        -ls || wc
    */
    while(tmp)
    {
        if(tmp->type == TOKEN_PIPE)
        {
            if(!tmp->next)
            {
                print_syntax_message("|'", env);
                return 0;
            }
            if(tmp->next->type == TOKEN_PIPE)
            {
                print_syntax_message("|'", env);
                return 0;
            }
            if(tmp->prev && tmp->prev->type == TOKEN_PIPE)
            {
                print_syntax_message("|'", env);
                return 0;
            }
        }
        tmp = tmp->next;
    }
    return 1;
}

int validate_syntax(t_token *tokens, t_env *env)
{
    if(!tokens)
        return 1;
    if(!validate_redirect_syntax(tokens, env))
    {

        return 0;
    }
    if(!validate_pipe_syntax(tokens,env))
    {

        return 0;
    }
    return 1;
}
