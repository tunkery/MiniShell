/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bolcay <bolcay@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/19 14:29:20 by hpehliva          #+#    #+#             */
/*   Updated: 2025/03/23 12:05:24 by bolcay           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"


char *expand_env(char *line, int *i)
{
    char *value;
    char var_name[256];
    int j;

    j = 0;
    (*i)++; // skip the '$'
    while(line[*i] && (ft_isalnum(line[*i]) || line[*i] == '_'))
    {
        var_name[j++] = line[*i];
        (*i)++;
    }
    var_name[j] = '\0';
    DEBUG_PRINT(CYAN"Expanding env var: '%s'\n"RESET, var_name);
    value = getenv(var_name);
    DEBUG_PRINT(CYAN"Found value: '%s'\n"RESET, value ? value : "NULL");
    if(value)
        return (ft_strdup(value));
    return ft_strdup("");
}

char *process_quoted(char *line, int *i, char quote_type)
{
    int start = *i + 1;
    char *result = ft_strdup("");
    char *temp;

    (*i)++;
    while(line[*i] && line[*i] != quote_type)
    {
        if(quote_type == '"' && line[*i] == '$')
        {
            temp = expand_env(line, i);
            if(temp)
                result = ft_strjoin(result, temp);
            free(temp);
        }
        else
        {
            char cpy[2] = {line[*i], '\0'};
            result = ft_strjoin(result, cpy);
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
    int start = *i;
    char *result = ft_strdup("");
    char *temp;

    while(line[*i] && line[*i] != ' ' && line[*i] != '|' && line[*i] != '<' && line[*i] != '>' && line[*i] != '\'' && line[*i] != '"')
    {
        if(line[*i] == '$')
        {
            temp = expand_env(line, i);
            if(temp)
               result = ft_strjoin(result, temp);
            free(temp);
        }
        else
        {
            char cpy[2] = {line[*i], '\0'};
            result = ft_strjoin(result, cpy);
            (*i)++;
        }
    }
    DEBUG_PRINT(RED"Extracted word: '%s' (start = %d, end = %d) \n"RESET, result, start, *i);
    return (result);
}

void seperated_token(char *line, t_token **head)
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
        // create a new token
        token = malloc(sizeof(t_token));
        if(!token)
        {
            DEBUG_PRINT(RED"Malloc failed\n"RESET);
            free_token_matrix(*head); // free the token matrix when malloc is failed
            *head = NULL;
            return;
        }

        if(line[i] == '|')
        {
            token->type = TOKEN_PIPE;
            token->value = ft_strdup("|");
            i++;
        }
        else if(line[i] == '<')
        {
            if(line[i + 1] == '<')
            {
                token->type = TOKEN_HEREDOC;
                token->value = ft_strdup("<<");
                i += 2;
            }
            else
            {
                token->type = TOKEN_REDIRECT_IN;
                token->value = ft_strdup("<");
                i++;
            }
        }
        else if(line[i] == '>')
        {
            if(line[i + 1] == '>')
            {
                token->type = TOKEN_REDIRECT_APPEND;
                token->value = ft_strdup(">>");
                i += 2;
            }
            else
            {
                token->type = TOKEN_REDIRECT_OUT;
                token->value = ft_strdup(">");
                i++;
            }
        }
        else if(line[i] == ';')
        {
            token->type = TOKEN_SEMIC;
            token->value = ft_strdup(";");
            i++;
        }
        else if(line[i] == '"')
        {
            token->type = TOKEN_WORD;
            token->value = process_quoted(line, &i, '"');
        }
        else if(line[i] == '\'') 
        {
            token->type = TOKEN_WORD;
            token->value = process_quoted(line, &i, '\'');
        }
        else // without quote
        {
            token->type = TOKEN_WORD;
            token->value = extract_word(line, &i);
        }
        if(!token->value)
        {
            DEBUG_PRINT(RED"FAILED TO EXTRACT WORD AT POSITION %d\n", i);
            free(token);
            free_token_matrix(*head);
            *head = NULL;
            return;
        }
        token->next = NULL;
        DEBUG_PRINT(RED"Created token: type= %d, value = '%s' \n", token->type, token->value);
        if(!*head)
            *head = token;
        else
            current->next = token;
        current = token;
    }
    DEBUG_PRINT(RED"Token seperation completed\n"RESET);
}

t_token    *tokenizer(char *line)
{
    t_token *head = NULL;


    /* Be sure this is symbol or word */
    seperated_token(line, &head);
    if(!head)
        DEBUG_PRINT(RED"No tokens created\n"RESET);
    else
        DEBUG_PRINT(GRN"Tokenizer finished, head token: type= %d, value = '%s'\n", head->type, head->value);
    return (head);
}
