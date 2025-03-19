/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hpehliva <hpehliva@student.42heilbronn.de  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/19 14:29:20 by hpehliva          #+#    #+#             */
/*   Updated: 2025/03/19 14:29:21 by hpehliva         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void free_token_matrix(t_token *token)
{
    t_token *temp;

    DEBUG_PRINT(RED"Freeing token matrix\n"RESET);
    while(token)
    {
        temp = token;
        token = token->next;
        DEBUG_PRINT(RED"Freeing token: type+%d, value= '%s'\n"RESET, temp->type, temp->value);
        free(temp->value); // free the vaue of token
        free(temp); // free the struct of token
    }
      
}

char    *extract_word( char *line, int *i)
{
    int start = *i;
    while(line[*i] && line[*i] != ' ' && line[*i] != '|' && line[*i] != '<' && line[*i] != '>')
        (*i)++;
    char *word = ft_substr(line, start, *i - start);
    DEBUG_PRINT(RED"Extracted word: '%s' (start = %d, end = %d) \n"RESET, word, start, *i);
    return (word);
}

void seperated_token(char *line, t_token **head)
{
    t_token *current;
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
            token->type = TOKEN_REDIRECT_IN;
            token->value = ft_strdup("<");
            i++;
        }
        else if(line[i] == '>')
        {
            token->type = TOKEN_REDIRECT_OUT;
            token->value = ft_strdup(">");
            i++;
        }
        else
        {
            token->type = TOKEN_WORD;
            token->value = extract_word(line, &i);
            if(!token->value)
            {
                DEBUG_PRINT(RED"FAILED TO EXTRACT WORD AT POSITION %d\n", i);
                free(token);
                free_token_matrix(*head);
                *head = NULL;
                return;
            }
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
    // t_token *current;
    // t_token *token;

    /* Be sure this is symbol or word */
    seperated_token(line, &head);
    if(!head)
        DEBUG_PRINT(RED"No tokens created\n"RESET);
    else
        DEBUG_PRINT(RED"Tokenizer finished, head token: type= %d, value = '%s'\n", head->type, head->value);
    return (head);
}
