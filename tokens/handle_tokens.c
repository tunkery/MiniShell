/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_tokens.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bolcay <bolcay@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/02 12:35:28 by hpehliva          #+#    #+#             */
/*   Updated: 2025/04/18 14:23:02 by bolcay           ###   ########.fr       */
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
    char *result = ft_strdup("");
    char *temp;

    token->type = TOKEN_WORD;

    while(line[*i] && line[*i] != ' ' && line[*i] != '|' && line[*i] != '>' && line[*i] != '<' && line[*i] != ';')
    {
        if(line[*i] == '"')
        {
            temp = process_quoted(line, i, '"', env);
            if(temp)
            {
                char *old_res = result;
                result = ft_strjoin(result, temp);
                free(old_res);
                free(temp);
            }
            else
            {
                free(result);
                token->value = NULL;
                return;
            }
        }
        else if(line[*i] =='\'')
        {
            temp = process_quoted(line, i, '\'', env);
            if(temp)
            {
                char *old_res = result;
                result = ft_strjoin(result, temp);
                free(old_res);
                free(temp);
            }
            else
            {
                free(result);
                token->value = NULL;
                return;
            }
        }
        else if (line[*i] == '$' && (line[*i + 1] && (ft_isalnum(line[*i + 1]) || line[*i + 1] == '_' || line[*i + 1] == '?')))
        {
            temp = expand_env(line, i, env);
            if(temp)
            {
                char *old_res = result;
                result = ft_strjoin(result, temp);
                free(old_res);
                free(temp);
            }
            else
            {
                char *old_res = result;
                result = ft_strjoin(result, temp);
                free(old_res);
            }
        }
        else if(line[*i] == '~' &&(line[*i + 1] == '/' || line[*i + 1] == '\0' || line[*i+1] == ' '))
        {
            temp = handle_tilde(line, i, env);
            if(temp)
            {
                char *old_res = result;
                result = ft_strjoin(result, temp);
                free(old_res);
                free(temp);
            }
        }
        else
        {
            char cpy[2] = {line[*i], '\0'};
            char *old_res = result;
            result = ft_strjoin(result, cpy);
            free(old_res);
            (*i)++;
        }

   }
   token->value = result;
}
