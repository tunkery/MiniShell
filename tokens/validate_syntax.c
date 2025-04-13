/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   validate_syntax.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hpehliva <hpehliva@student.42heilbronn.de  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/13 18:38:08 by hpehliva          #+#    #+#             */
/*   Updated: 2025/04/13 18:38:10 by hpehliva         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int validate_redirect_syntax(t_token *tokens)
{
    t_token *tmp = tokens;

    while(tmp)
    {
        if(tmp->type == TOKEN_REDIRECT_IN || tmp->type == TOKEN_REDIRECT_OUT || tmp->type == TOKEN_REDIRECT_APPEND || tmp->type == TOKEN_HEREDOC)
        {
            if(!tmp->next)
            {
                printf("minishell: syntax error near unexpected token `newline'\n");
                return 0;
            }
            if(tmp->next->type == TOKEN_WORD)
            {
                printf("minishell: syntax error near unexpected token `%s'\n", tmp->next->value);
                return 0;
            }
        }
        tmp = tmp->next;
    }
    return 1;
}
