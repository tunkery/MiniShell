/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bolcay <bolcay@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/19 14:29:20 by hpehliva          #+#    #+#             */
/*   Updated: 2025/04/02 16:26:45 by bolcay           ###   ########.fr       */
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
        printf("Exit status: %s\n", exit_status);
        // free(exit_status); // don't need to free it here!.
        return (exit_status);
    }

    while(line[*i] && (ft_isalnum(line[*i]) || line[*i] == '_'))
    {
        var_name[j++] = line[*i];
        (*i)++;
    }
    var_name[j] = '\0';
    DEBUG_PRINT(CYAN"Expanding env var: '%s'\n"RESET, var_name);

    // try to get env structure!
    // int k = 0;
    // while (env->envp[k])
    // {
    //     // Extract key from env variable
    //     size_t key_len = 0;
    //     while (env->envp[k][key_len] && env->envp[k][key_len] != '=')
    //         key_len++;
        
    //     // Compare key with var_name
    //     if (key_len == ft_strlen(var_name) && 
    //         ft_strncmp(env->envp[k], var_name, key_len) == 0)
    //     {
    //         // Return value part (after the '=')
    //         return ft_strdup(env->envp[k] + key_len + 1);
    //     }
    //     k++;
    // }
    
    // Fallback to getenv
    value = getenv(var_name);
    if(value)
        return (ft_strdup(value));
    return ft_strdup("");
}

char *process_quoted(char *line, int *i, char quote_type, t_env *env)
{
    int start = *i + 1;
    char *result = ft_strdup("");
    char *temp;

    (*i)++;
    while(line[*i] && line[*i] != quote_type)
    {
        if(quote_type == '"' && line[*i] == '$')
        {
            DEBUG_PRINT(RED"Hello I'm here"RESET);
            temp = expand_env(line, i, env);
            if(temp)
            {
                char *old_res = result;
                result = ft_strjoin(result, temp);
                free(old_res);
                free(temp);
            }
            // BEFORE
            //     result = ft_strjoin(result, temp);
            // free(temp);
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
        DEBUG_PRINT(RED"Failed to process quoted string\n"RESET);
        return (NULL);
    }
    DEBUG_PRINT(RED"Processed quoted string: '%s' (start = %d, end = %d)\n"RESET, result, start, *i);
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
    // DEBUG_PRINT(RED"Extracted word: '%s' (start = %d, end = %d) \n"RESET, result, start, *i);
    return (result);
}


t_token *handle_special_token(char *line, int *i, t_env *env)
{
    t_token *token;

    token = malloc(sizeof(t_token));
    if(!token)
    {
        DEBUG_PRINT(MGNT"Memory allocated failed for tokens!\n"RESET);
        return NULL;
    }
    token->next = NULL;
    token->prev = NULL;
    if(line[*i] == '|')
        handle_pipe(token, i);
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
        DEBUG_PRINT(RED"FAILED TO EXTRACT WORD AT POSITION %d\n", *i);
        free(token);
        return NULL;
    }
    token->next = NULL;
    DEBUG_PRINT(RED"Created token: type= %d, value = '%s' \n", token->type, token->value);
    return token;    
}

void seperated_token(char *line, t_token **head, t_env *env)
{
    t_token *current = NULL;
    t_token *token;
    int i = 0;

    DEBUG_PRINT(RED"Starting token seperation for line: '%s'\n", line);
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
            DEBUG_PRINT(RED"FAILED TO EXTRACT WORD AT POSITION %d\n", i);
            free_token_matrix(*head);
            *head = NULL;
            return;
        }
        DEBUG_PRINT(RED"Created token: type= %d, value = '%s' \n", token->type, token->value);

        token->prev = current;

        if(!*head)
            *head = token;
        else
            current->next = token;
        current = token;
    }
    // DEBUG_PRINT(RED"Token seperation completed\n"RESET);
}

t_token    *tokenizer(char *line, t_env *env)
{
    t_token *head = NULL;


    /* Be sure this is symbol or word */
    seperated_token(line, &head, env);
    if(!head)
        DEBUG_PRINT(RED"No tokens created\n"RESET);
    else
        DEBUG_PRINT(GRN"Tokenizer finished, head token: type= %d, value = '%s'\n", head->type, head->value);
    return (head);
}
