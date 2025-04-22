/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_tokens.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: batuhan <batuhan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/02 12:35:28 by hpehliva          #+#    #+#             */
/*   Updated: 2025/04/22 16:15:00 by batuhan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"


void    handle_pipe(t_token *token, int *i, t_env *env)
{
    token->type = TOKEN_PIPE;
    token->value = ft_strdup("|");
    gc_register(env->s_gc, token->value);
    (*i)++;
}

void    handle_redirect_in(t_token *token, char *line, int *i, t_env *env)
{
    if(line[*i + 1] == '<')
    {
        token->type = TOKEN_HEREDOC;
        token->value = ft_strdup("<<");
        gc_register(env->s_gc, token->value);

        *i += 2;

        while(line[*i] && line[*i] == ' ')
            (*i)++;

        if(line[*i] == '\'')
        {
            token->value = ft_strdup("<<'");
            gc_register(env->s_gc, token->value);
            // (*i)++;
        }
        else
        {
            token->value = ft_strdup("<<");
            gc_register(env->s_gc, token->value);
        }
        

    }
    else
    {
        token->type = TOKEN_REDIRECT_IN;
        token->value = ft_strdup("<");
        gc_register(env->s_gc, token->value);
        (*i)++;
    }
}

void    handle_redirect_out(t_token *token, char *line, int *i, t_env *env)
{

    if(line[*i + 1] == '>')
    {
        token->type = TOKEN_REDIRECT_APPEND;
        token->value = ft_strdup(">>");
        gc_register(env->s_gc, token->value);
        (*i) += 2;
    }
    else
    {
        token->type = TOKEN_REDIRECT_OUT;
        token->value = ft_strdup(">");
        gc_register(env->s_gc, token->value);
        (*i)++;
    }
}

void    handle_semic(t_token *token, int *i, t_env *env)
{
    token->type = TOKEN_SEMIC;
    token->value = ft_strdup(";");
    gc_register(env->s_gc, token->value);
    (*i)++;
}


void    handle_word(t_token *token, char *line, int *i, t_env *env)
{
    char *result = ft_strdup("");
    char *temp;

    gc_register(env->s_gc, result);
    token->type = TOKEN_WORD;

    while(line[*i] && line[*i] != ' '  && line[*i] != '\t' && line[*i] != '|' && line[*i] != '>' && line[*i] != '<' && line[*i] != ';')
    {
        //Add backslash rules!
        if(line[*i] == '\\')
        {
            (*i)++;
            if(!line[*i])
            {
                // free(result);
                token->value =NULL;
                return;
            }
            if(line[*i] == '\\' && line[*i + 1] == '$')
            {
                (*i)++;
                char cpy[2] = {'\\', '\0'};
                // char *old_res = result;
                result = ft_strjoin(result, cpy);
                gc_register(env->s_gc, result);
                // free(old_res);
                continue;
            }
            char cpy[2] = {line[*i], '\0'};
            // char *old_res = result;
            result = ft_strjoin(result, cpy);
            gc_register(env->s_gc, result);
            // free(old_res);
            (*i)++;
            continue;
        }
        if(line[*i] == '"')
        {
            temp = process_quoted(line, i, '"', env);
            if(temp)
            {
                // char *old_res = result;
                result = ft_strjoin(result, temp);
                gc_register(env->s_gc, result);
                // free(old_res);
                // free(temp);
            }
            else
            {
                // free(result);
                token->value = NULL;
                return;
            }
        }
        else if(line[*i] =='\'')
        {
            temp = process_quoted(line, i, '\'', env);
            if(temp)
            {
                // char *old_res = result;
                result = ft_strjoin(result, temp);
                gc_register(env->s_gc, result);
                // free(old_res);
                // free(temp);
            }
            else
            {
                // free(result);
                token->value = NULL;
                return;
            }
        }
        else if (line[*i] == '$' && (line[*i + 1] && (ft_isalnum(line[*i + 1]) || line[*i + 1] == '_' || line[*i + 1] == '?' || line[*i + 1] == '"')))
        {
            temp = expand_env(line, i, env);
            if(temp)
            {
                // char *old_res = result;
                result = ft_strjoin(result, temp);
                gc_register(env->s_gc, result);
                // free(old_res);
                // free(temp);
            }
            else
            {
                // char *old_res = result;
                result = ft_strjoin(result, temp);
                gc_register(env->s_gc, result);
                // free(old_res);
            }
        }
        else if(line[*i] == '~' &&(line[*i + 1] == '/' || line[*i + 1] == '\0' || line[*i+1] == ' '))
        {
            temp = handle_tilde(line, i, env);
            if(temp)
            {
                // char *old_res = result;
                result = ft_strjoin(result, temp);
                gc_register(env->s_gc, result);
                // free(old_res);
                // free(temp);
            }
        }
        else
        {
            char cpy[2] = {line[*i], '\0'};
            // char *old_res = result;
            result = ft_strjoin(result, cpy);
            gc_register(env->s_gc, result);
            // free(old_res);
            (*i)++;
        }

   }
   token->value = result;
}
