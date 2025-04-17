/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   deallocation.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bolcay <bolcay@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 10:33:02 by bolcay            #+#    #+#             */
/*   Updated: 2025/04/17 15:30:44 by bolcay           ###   ########.fr       */
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
        str[i] = NULL;
		i++;
	}
	free(str);
}

void free_token_matrix(t_token *token)
{
    t_token *temp;

    while(token)
    {
        temp = token;
        token = token->next;
        free(temp->value); // free the vaue of token
        free(temp); // free the struct of token
    }
      
}