/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bolcay <bolcay@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 10:34:55 by bolcay            #+#    #+#             */
/*   Updated: 2025/03/17 12:22:21 by bolcay           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static	int	ft_count(const char *str, int j)
{
	int	i;
	int	count;

	i = 0;
	count = 0;
	while (str[i])
	{
		while (str[i] == j)
			i++;
		if (str[i] && str[i] != j)
		{
			count++;
			while (str[i] && str[i] != j)
				i++;
		}
	}
	return (count);
}

char **get_tokens(char **str)
{
	int i;
	char **tokens;
	int size;

	size = ft_count(*str, ' ');
	tokens = malloc(sizeof(char **) * size);
	i = 0;
	while (str[i])
	{
		tokens[i] = ft_strdup(str[i]);
		i++;
	}
	return (tokens);
}
