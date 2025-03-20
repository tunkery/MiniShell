/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   deallocation.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bolcay <bolcay@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 10:33:02 by bolcay            #+#    #+#             */
/*   Updated: 2025/03/17 12:20:31 by bolcay           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	clean_2d(char **str)
{
	int i;

	if(!str)
		return ;
	i = 0;
	while (str[i])
	{
		free(str[i]);
		i++;
	}
	free(str);
}

void free_token_matrix(t_token *token)
{
    t_token *temp;

    DEBUG_PRINT(RED"Freeing token matrix\n"RESET);
    while(token)
    {
        temp = token;
        token = token->next;
        DEBUG_PRINT(RED"Freeing token: type=%d, value= '%s'\n"RESET, temp->type, temp->value);
        free(temp->value); // free the vaue of token
        free(temp); // free the struct of token
    }
      
}