/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_tokens.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hpehliva <hpehliva@student.42heilbronn.de  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/02 12:35:28 by hpehliva          #+#    #+#             */
/*   Updated: 2025/04/02 12:35:29 by hpehliva         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void    handle_pipe(t_token *token, int *i)
{
    token->type = TOKEN_PIPE;
    token->value = ft_strdup("|");
    (*i)++;
}

void    handle_redirect_in(t_token *token, char *line, int *i)
{
    if(line[*i + 1] == '<')
    {
        token->type = TOKEN_HEREDOC;
        token->value = ft_strdup("<<");
        *i += 2;
    }
    else
    {
        token->type = TOKEN_REDIRECT_IN;
        token->value = ft_strdup("<");
        (*i)++;
    }
}

void    handle_redirect_out(t_token *token, char *line, int *i)
{

    if(line[*i + 1] == '>')
    {
        token->type = TOKEN_REDIRECT_APPEND;
        token->value = ft_strdup(">>");
        (*i) += 2;
    }
    else
    {
        token->type = TOKEN_REDIRECT_OUT;
        token->value = ft_strdup(">");
        (*i)++;
    }
}

void    handle_semic(t_token *token, int *i)
{
    token->type = TOKEN_SEMIC;
    token->value = ft_strdup(";");
    (*i)++;
}

void    handle_word(t_token *token, char *line, int *i, t_env *env)
{
    if(line[*i] == '"')
    {
        token->type = TOKEN_WORD;
        token->value = process_quoted(line, i, '"', env);
    }
    else if(line[*i] == '\'') 
    {
        token->type = TOKEN_WORD;
        token->value = process_quoted(line, i, '\'', env);
    }
    else // without quote
    {
        if(line[*i] == '$')
        {
            token->value = expand_env(line, i, env);
            if(!token->value)
            {
                DEBUG_PRINT(RED"Failed to process env variable\n"RESET);
                free(token);
                return ;
            }
        }
        else
        {
            token->type = TOKEN_WORD;
            token->value = extract_word(line, i);
        }
    }
}
