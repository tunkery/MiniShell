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

    
    // İşlem yapacağımız token zaten oluşturulmuş olmalı
    char *result = ft_strdup("");
    gc_register(env->s_gc, result);
    
    // Tırnak kontrolü
    int quote_mode = 0;
    if (line[*i] == '\'')
    {
        quote_mode = 1;
        (*i)++;  // Tırnağı atla
        
        // Tırnak içeriğini al (tırnak kapanana kadar)
        while (line[*i] && line[*i - 1] == '\'')
        {
            char cpy[2] = {line[*i], '\0'};
            result = ft_strjoin(result, cpy);
            gc_register(env->s_gc, result);
            // (*i)++;
        }
        
        if (line[*i] == '\'')
            (*i)++;  // Kapanış tırnağını atla
    }
    else if (line[*i] == '"')
    {
        quote_mode = 1;
        (*i)++;  // Tırnağı atla
        
        // Tırnak içeriğini al (tırnak kapanana kadar)
        while (line[*i] && line[*i] != '"')
        {
            char cpy[2] = {line[*i], '\0'};
            result = ft_strjoin(result, cpy);
            gc_register(env->s_gc, result);
            // (*i)++;
        }
        
        if (line[*i] == '"')
            (*i)++;  // Kapanış tırnağını atla
    }
    else
    {
        quote_mode = 0;
        // Tırnaksız delimiter
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
    
    // Delimiter değerini token'a ata
    token->value = result;
    token->quote_mode = quote_mode;  // quote_mode'u t_token yapısına ekleyin
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
            

            
        // // Satır sonuna gelindiyse veya özel karakter geldiyse
        // if(!line[*i] || line[*i] == '\n' || line[*i] == '|' || 
        //   line[*i] == '<' || line[*i] == '>' || line[*i] == ';')
        // {

        //     // Delimiter yok, sözdizimi hatası olacak
        //     return;
        // }
        // token->quote_mode = 0;
        // if(line[*i] == '\'')
        // {
        //     token->quote_mode = 1;
        //     token->value = ft_strdup("<<'");
        //     gc_register(env->s_gc, token->value);
        //     // (*i)++;
        // }
        // else if(line[*i] == '"')
        // {
        //     token->quote_mode = 1;
        //     token->value = ft_strdup("<<\"");
        //     gc_register(env->s_gc, token->value);
        //     // (*i)++;
        // }
        // else
        // {
        //     token->value = ft_strdup("<<");
        //     gc_register(env->s_gc, token->value);
        // }
        

        
        // Delimiter token'ı oluştur
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

        
        // Normal < işareti
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
        if(line[*i] == '\"')
        {
            temp = process_quoted(line, i, '\"', env);
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
