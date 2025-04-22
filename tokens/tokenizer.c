/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bolcay <bolcay@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/19 14:29:20 by hpehliva          #+#    #+#             */
/*   Updated: 2025/04/18 16:10:38 by bolcay           ###   ########.fr       */
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
            return value;
        }
        i++;
    }
    return NULL;
}

char *expand_env(char *line, int *i, t_env *env)
{
    char *value;
    char var_name[256];
    char *exit_status;
    int j;

    j = 0;
    (*i)++; 

    if(line[*i] == '?')
    {
        (*i)++;
        exit_status = ft_itoa(env->exit_code);
        return (exit_status);
    }

    while(line[*i] && (ft_isalnum(line[*i]) || line[*i] == '_'))
    {
        var_name[j++] = line[(*i)++];
    }
    var_name[j] = '\0';
    value = get_path(var_name, env);
    if(value)
        return (value);
    return ft_strdup("");
}

char *process_quoted(char *line, int *i, char quote_type, t_env *env)
{
    char *result = ft_strdup("");
    char *temp;

    (*i)++;
    while(line[*i] && line[*i] != quote_type)
    {
        if(quote_type == '"' && line[*i] == '$' && line[*i + 1] && (ft_isalnum(line[*i + 1]) || line[*i + 1] == '_' || line[*i + 1] == '?'))
        {

                temp = expand_env(line, i, env);
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
    if(line[*i] == quote_type)
        (*i)++;
    else
    {
        free(result);
        return (NULL);
    }
    return (result);
}



char    *extract_word( char *line, int *i)
{

    char *result = ft_strdup("");
    while(line[*i] && line[*i] != ' ' && line[*i] != '|' && line[*i] != '<' && line[*i] != '>' && line[*i] != '\'' && line[*i] != '"' && line[*i] != ';')
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
        while(line[i] && (line[i] == ' ' || line[i] == '\t'))
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

    seperated_token(line, &head, env);
    return (head);
}
