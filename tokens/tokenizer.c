/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: batuhan <batuhan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/19 14:29:20 by hpehliva          #+#    #+#             */
/*   Updated: 2025/04/15 17:46:41 by batuhan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"


char *expand_env(char *line, int *i, t_env *env)
{
    char *value;
    char var_name[256];
    char *exit_status;
    int j;

    j = 0;
    (*i)++; // skip the '$'

    if(line[*i] == '?')
    {
        (*i)++;
        exit_status = ft_itoa(env->exit_code);
        return (exit_status);
    }

    while(line[*i] && (ft_isalnum(line[*i]) || line[*i] == '_'))
    {
        var_name[j++] = line[*i];
        (*i)++;
    }
    var_name[j] = '\0';
    // Fallback to getenv
    value = getenv(var_name);
    if(value)
        return (ft_strdup(value));
    return ft_strdup("");
}

char *process_quoted(char *line, int *i, char quote_type, t_env *env)
{
    // int start = *i + 1;
    char *result = ft_strdup("");
    char *temp;

    (*i)++;
    while(line[*i] && line[*i] != quote_type)
    {
        if(quote_type == '"' && line[*i] == '$')
        {
            temp = expand_env(line, i, env);
            if(temp)
            {
                char *old_res = result;
                result = ft_strjoin(result, temp);
                free(old_res);
                free(temp);
            } // check it all free as lldb.
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
    if(line[*i] == quote_type)
        (*i)++;
    else // single quote doesn't have expantation.
    {
        free(result);
        return (NULL);
    }
    return (result);
}



char    *extract_word( char *line, int *i)
{
    // int start = *i;
    char *result = ft_strdup("");
    // char *temp;

    while(line[*i] && line[*i] != ' ' && line[*i] != '|' && line[*i] != '<' && line[*i] != '>' && line[*i] != '\'' && line[*i] != '"')
    {
        char cpy[2] = {line[*i], '\0'};
        char *old_res = result;
        result = ft_strjoin(result, cpy);
        free(old_res);
        (*i)++;

    }
    return (result);
}


t_token *handle_special_token(char *line, int *i, t_env *env)
{
    t_token *token;

    token = malloc(sizeof(t_token));
    if(!token)
    {
        return NULL;
    }
    token->next = NULL;
    token->prev = NULL;
    if(line[*i] == '|')
        handle_pipe(token,i);
    else if(line[*i] == '<')
        handle_redirect_in(token, line, i);
    else if(line[*i] == '>')
        handle_redirect_out(token, line, i);
    else if(line[*i] == ';')
        handle_semic(token, i);
    else
        handle_word(token, line, i,env);
    if(!token->value)
    {
        free(token);
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
        // split spaces
        while(line[i] == ' ')
            i++;
        if(line[i] == '\0')
            break;
        token = handle_special_token(line, &i, env);
        if(!token)
        {
            free_token_matrix(*head);
            *head = NULL;
            return;
        }

        token->prev = current;

        if(!*head)
            *head = token;
        else
            current->next = token;
        current = token;
    }
}

t_token    *tokenizer(char *line, t_env *env)
{
    t_token *head = NULL;


    /* Be sure this is symbol or word */
    seperated_token(line, &head, env);
    return (head);
}
