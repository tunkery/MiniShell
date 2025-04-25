/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: batuhan <batuhan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/19 14:29:20 by hpehliva          #+#    #+#             */
/*   Updated: 2025/04/22 18:21:30 by batuhan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"



char  *handle_tilde(char *line, int *i, t_env *env)
{
    int j = 0;
    char *result = NULL;
    char *home_path = NULL;

    ((*i)++); // skip the tilde
    while(env->envp[j] && ft_strncmp(env->envp[j], "HOME=", 5) != 0)
        j++;
    if(env->envp[j])
        home_path = ft_substr(env->envp[j], 5, ft_strlen(env->envp[j]) - 5);
    else
        env->envp[j] = "~";
    
    if(line[*i] == '/' || line[*i] == '\0' || line[*i] == ' ')
        result = ft_strdup(home_path);
    else
        result = ft_strdup("~");
    free(home_path);
    gc_register(env->s_gc, result);
    return(result);

}

static char	*get_path(char *str, t_env *env)
{
    int i;
    int len;
    char *value;

    len = ft_strlen(str);
    i = 0;
    while(env->envp[i])
    {
        if(ft_strncmp(env->envp[i], str, len) == 0 && env->envp[i][len] == '=')
        {
            value = ft_substr(env->envp[i], len+1, ft_strlen(env->envp[i])-len - 1);
            gc_register(env->s_gc, value);
            return value;
        }
        i++;
    }
    return NULL;
}

char get_escape(char c)
{
    if(c == 'n')
        return '\n';
    else if (c == 't')
        return '\t';
    else if (c == 'r')
        return '\r';
    else if (c == 'a')
        return '\a';
    else if (c == 'b')
        return '\b';
    else if (c == 'f')
        return '\f';
    else if (c == 'v')
        return '\v';
    else if(c == '\\')
        return '\\';
    else if(c == '\'')
        return '\'';
    else if(c == '"')
        return '"';
    else if(c == '$')
        return c;
    else
        return c;
}

char *ansi_c_quote(char *str, t_env *env)
{
    char *result = ft_strdup("");
    int i ;
    char escape[2];

    i = 0;
    escape[0] = 0;
    escape[1] = 0;
    gc_register(env->s_gc,result);
    while(str[i])
    {
        if(str[i] == '\\')
        {
            i++;
            if(!str[i])
                break;
            escape[0] = get_escape(str[i]);
            result =ft_strjoin(result,escape);
            gc_register(env->s_gc,result);
        }
        else
        {
            char cpy[2] = {str[i],'\0'};
            result = ft_strjoin(result,cpy);
            gc_register(env->s_gc,result);
        }
        i++;
    }
    return result;
}


static char *expand_exit_status(int *i,t_env *env)
{
    char *exit_status;

    (*i)++;
    exit_status = ft_itoa(env->exit_code);
    gc_register(env->s_gc, exit_status);
    return (exit_status);
}
static char *expand_quote_cond(char *line, int *i, char quote_str)
{
    char quote_status[1024];
    int quo_i = 0;

    while(line[*i] && line[*i] != quote_str)
        quote_status[quo_i++] = line[(*i)++];
    quote_status[quo_i] = '\0';
    if(line[*i] == quote_str)
        (*i)++;
    return ft_strdup(quote_status);
}

static char *quote_expansion(char *line,int *i,char quote_str, t_env *env)
{
    char *quote_status;
    char *result;

    quote_status = expand_quote_cond(line,i,quote_str);
    result = ansi_c_quote(quote_status,env);
    free(quote_status);
    return result;
}

static char *variable_expansion(char *line, int *i, t_env *env)
{
    char var_name[256];
    char *value;
    int j;

    j = 0;
    while(line[*i] && (ft_isalnum(line[*i]) || line[*i] == '_'))
    {
        var_name[j++] = line[(*i)++];
    }
    var_name[j] = '\0';
    value = get_path(var_name, env);
    if(!value)
    {
        value = ft_strdup("");
        gc_register(env->s_gc, value);
    }
    //     return (value);
    // else
    // {
    //     value = ft_strdup("");
    // }
    // gc_register(env->s_gc, value);
    return (value);
}

char *expand_env(char *line, int *i, t_env *env)
{

    (*i)++;

    if(line[*i] == '?')
        return expand_exit_status(i,env);
    else if(line[*i] == '\'')
        return quote_expansion(line,i,'\'',env);
    // else if(line[*i] == '"')
    //     return quote_expansion(line,i,'"',env);

    return variable_expansion(line,i,env);
    
    // char *value;
    // char var_name[256];
    // char *exit_status;
    // int j;

    // j = 0;
    // (*i)++; 

    // if(line[*i] == '?')
    // {
    //     (*i)++;
    //     exit_status = ft_itoa(env->exit_code);
    //     gc_register(env->s_gc, exit_status);
    //     return (exit_status);
    // }

    // while(line[*i] && (ft_isalnum(line[*i]) || line[*i] == '_'))
    // {
    //     var_name[j++] = line[(*i)++];
    // }
    // var_name[j] = '\0';
    // value = get_path(var_name, env);
    // if(value)
    //     return (value);
    // else
    // {
    //     value = ft_strdup("");
    // }
    // gc_register(env->s_gc, value);
    // return (value);
}

char *process_quoted(char *line, int *i, char quote_type, t_env *env)
{
    char *result = ft_strdup("");
    char *temp;

    gc_register(env->s_gc, result);
    (*i)++;
    while(line[*i] && line[*i] != quote_type)
    {
        if(quote_type == '"' && line[*i] == '$' && line[*i + 1] && 
            (ft_isalnum(line[*i + 1]) || line[*i + 1] == '_' || line[*i + 1] == '?'))
        {

            temp = expand_env(line, i, env);
            if(temp)
            {
                result = ft_strjoin(result, temp);
                gc_register(env->s_gc, result);
            }

        }
        else
        {
            char cpy[2] = {line[*i], '\0'};
            result = ft_strjoin(result, cpy);
            gc_register(env->s_gc, result);
            (*i)++;
        }
    }
    if(line[*i] == quote_type)
        (*i)++;
    else
        return (NULL);
    return (result);
}



char    *extract_word( char *line, int *i, t_env *env)
{

    char *result = ft_strdup("");
    gc_register(env->s_gc, result);
    while(line[*i] && line[*i] != ' ' && line[*i] != '|' && line[*i] != '<' && line[*i] != '>' && line[*i] != '\'' && line[*i] != '"' && line[*i] != ';')
    {
            char cpy[2] = {line[*i], '\0'};
            // char *old_res = result;
            result = ft_strjoin(result, cpy);
            gc_register(env->s_gc, result);
            // free(old_res);
            (*i)++;

    }
    return (result);
}


t_token *handle_special_token(char *line, int *i, t_env *env)
{
    t_token *token;

    token = my_malloc(env->s_gc, sizeof(t_token));
    if(!token)
    {
        return NULL;
    }
    token->next = NULL;
    if(line[*i] == '|')
        handle_pipe(token,i, env);
    else if(line[*i] == '<')
        handle_redirect_in(token, line, i, env);
    else if(line[*i] == '>')
        handle_redirect_out(token, line, i, env);
    else if(line[*i] == ';')
        handle_semic(token, i, env);
    else
        handle_word(token, line, i, env);
    if(!token->value)
    {
        // free(token);
        return NULL;
    }
    token->next = NULL;
    return token;    
}

void seperated_token(char *line, t_token **head, t_env *env)
{
      
    t_token *current = NULL;
    t_token *token;
    int i = 0;

    while(line[i])
    {
        while(line[i] && (line[i] == ' ' || line[i] == '\t'))
            i++;
        if(line[i] == '\0')
            break;
        
        // handle_special_token yerine farklı bir yaklaşım kullanın
        if(line[i] == '<' && line[i+1] == '<')  // Heredoc kontrol et
        {
            // Heredoc token oluştur
            token = my_malloc(env->s_gc, sizeof(t_token));
            if(!token) return;
            
            token->type = TOKEN_HEREDOC;
            token->value = ft_strdup("<<");
            gc_register(env->s_gc, token->value);
            
            // Token'ı listeye ekle
            token->prev = current;
            if(!*head)
                *head = token;
            else
                current->next = token;
            current = token;
            
            i += 2;  // << operatörünü atla
            
            // Boşlukları atla
            while(line[i] && (line[i] == ' ' || line[i] == '\t'))
                i++;
                
            // Satır sonu veya özel karakter geldi mi kontrol et
            if(!line[i] || line[i] == '\n' || line[i] == '|' || 
               line[i] == '<' || line[i] == '>' || line[i] == ';')
            {
                continue;  // Delimiter yok, validation'da hata verilecek
            }
            
            // Delimiter token oluştur
            t_token *delimiter = my_malloc(env->s_gc, sizeof(t_token));
            if(!delimiter) return;
            
            delimiter->type = TOKEN_WORD;
            delimiter->value = ft_strdup("");  // Boş değer başlat
            gc_register(env->s_gc, delimiter->value);
            
            // Delimiter içeriğini al
            char *result = delimiter->value;
            
            // Tırnak kontrolü ve içerik alma işlemleri...
            // (Mevcut heredoc_delimiter fonksiyonunuzdaki mantık burada uygulanabilir)
            
            while(line[i] && line[i] != ' ' && line[i] != '\t' && 
                  line[i] != '|' && line[i] != '<' && line[i] != '>' && 
                  line[i] != ';')
            {
                char cpy[2] = {line[i], '\0'};
                result = ft_strjoin(result, cpy);
                gc_register(env->s_gc, result);
                delimiter->value = result;
                i++;
            }
            
            // Delimiter token'ı listeye ekle
            delimiter->prev = current;
            delimiter->next = NULL;
            current->next = delimiter;
            current = delimiter;
        }
        else  // Diğer token türleri
        {
            token = handle_special_token(line, &i, env);
            if(!token) return;
            
            token->prev = current;
            if(!*head)
                *head = token;
            else
                current->next = token;
            current = token;
        }
    }
    
}

t_token    *tokenizer(char *line, t_env *env)
{
    t_token *head = NULL;

    seperated_token(line, &head, env);
    return (head);
}
