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

void heredoc_delimiter(t_token *token, char *line, int *i, t_env *env)
{
    char *result = ft_strdup("");
    gc_register(env->s_gc, result);
    
    int quote_mode = 0;
    if (line[*i] == '\'')
    {
        quote_mode = 1;
        (*i)++;

        while (line[*i] && line[*i - 1] == '\'')
        {
            char cpy[2] = {line[*i], '\0'};
            result = ft_strjoin(result, cpy);
            gc_register(env->s_gc, result);
        }
        
        if (line[*i] == '\'')
            (*i)++;
    }
    else if (line[*i] == '"')
    {
        quote_mode = 1;
        (*i)++;
        
        while (line[*i] && line[*i] != '"')
        {
            char cpy[2] = {line[*i], '\0'};
            result = ft_strjoin(result, cpy);
            gc_register(env->s_gc, result);
            // (*i)++;
        }
        if (line[*i] == '"')
            (*i)++;
    }
    else
    {
        quote_mode = 0;
        while (line[*i] && line[*i] != ' ' && line[*i] != '\t' && 
              line[*i] != '|' && line[*i] != '>' && line[*i] != '<' && 
              line[*i] != ';' && line[*i] != '\'' && line[*i] != '"')
        {
            char cpy[2] = {line[*i], '\0'};
            result = ft_strjoin(result, cpy);
            gc_register(env->s_gc, result);
            (*i)++;
        }
    }
    
    token->value = result;
    token->quote_mode = quote_mode; 
}


void    handle_redirect_in(t_token *token, char *line, int *i, t_env *env)
{

    
    if(line[*i + 1] == '<')
    {
        // Heredoc token'ı oluştur
        token->type = TOKEN_HEREDOC;
        token->value = ft_strdup("<<");
        gc_register(env->s_gc, token->value);
        *i += 2;
        
        // Boşlukları atla
        while(line[*i] && (line[*i] == ' ' || line[*i] == '\t'))
            (*i)++;
            

        t_token *delimiter_token = my_malloc(env->s_gc, sizeof(t_token));
        if (!delimiter_token)
            return;
            
        // Bağlantıları ayarla
        delimiter_token->type = TOKEN_WORD;
        delimiter_token->next = token->next;  // Heredoc'un next'i şimdi delimiter'ın next'i
        if (token->next)
            token->next->prev = delimiter_token;  // Eğer heredoc'un next'i varsa, onun prev'i artık delimiter
        token->next = delimiter_token;  // Heredoc'un next'i artık delimiter
        delimiter_token->prev = token;  // Delimiter'ın prev'i heredoc
        int quote_mode = 0;
        if(line[*i] == '\'')
            quote_mode = 1;
        else if (line[*i] == '"')
            quote_mode = 1;

        // Delimiter içeriğini işle
        heredoc_delimiter(delimiter_token, line, i, env);
        delimiter_token->quote_mode = quote_mode;

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

