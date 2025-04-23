/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   garbage_collector.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bolcay <bolcay@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/22 12:46:37 by batuhan           #+#    #+#             */
/*   Updated: 2025/04/23 17:14:01 by bolcay           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

t_gc	*gc_new(void)
{
	t_gc	*gc;

	gc = malloc(sizeof(t_gc));
	if (!gc)
		return (NULL);
	gc->head = NULL;
	return (gc);
}

void	*my_malloc(t_gc *gc, int size)
{
	void		*mem;
	t_gc_node	*node;

	mem = malloc(size);
	if (!mem)
		return (NULL);
	node = malloc(sizeof * node);
	if (!node)
	{
		free(mem);
		return (NULL);
	}
	node->ptr = mem;
	node->next = gc->head;
	gc->head = node;
	return (mem);
}

void	gc_register(t_gc *gc, void *ptr)
{
	t_gc_node	*node;
	t_gc_node	*check;

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

void	gc_free_all(t_gc *gc)
{
	t_gc_node	*curr;
	t_gc_node	*temp;

	curr = gc->head;
	while (curr)
	{
		free(curr->ptr);
		temp = curr;
		curr = curr->next;
		free(temp);
	}
	free(gc);
}
