/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   garbage_collector1.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hpehliva <hpehliva@student.42heilbronn.de  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/28 13:25:12 by hpehliva          #+#    #+#             */
/*   Updated: 2025/04/28 13:25:14 by hpehliva         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	gc_register(t_gc *gc, void *ptr)
{
	t_gc_node	*node;
	t_gc_node	*check;

	node = NULL;
	check = NULL;
	if (!gc || !ptr) 
		return ;
	check = gc->head;
	while (check)
	{
		if (check->ptr == ptr)
			return ;
		check = check->next;
	}
	node = malloc(sizeof * node);
	if (!node)
		return ;
	node->ptr = ptr;
	node->next = gc->head;
	gc->head = node;
}

void	gc_unregister(t_gc *gc, void *ptr)
{
	t_gc_node	**curr;
	t_gc_node	*rm;

	curr = &gc->head;
	while (*curr)
	{
		if ((*curr)->ptr == ptr)
		{
			rm = *curr;
			*curr = rm->next;
			free(rm);
			return ;
		}
		curr = &(*curr)->next;
	}
}
