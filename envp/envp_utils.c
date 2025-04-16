/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   envp_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: batuhan <batuhan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 09:11:50 by bolcay            #+#    #+#             */
/*   Updated: 2025/04/16 09:32:49 by batuhan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	key_size(char *str)
{
	int i;

	i = 0;
	while (str[i])
	{
		if (str[i] == '=')
			return (i - 1);
		i++;
	}
	return (i);
}

int	value_size(char *str)
{
	int i;
	int j;

	i = 0;
	j = 0;
	while (str[i] != '=')
		i++;
	while (str[i + j])
		j++;
	return (j);
}

int	env_size(char **envp)
{
	int i;

	i = 0;
	while (envp[i])
		i++;
	return (i);
}

void copy_env(char **str, char ***envp)
{
	int i;
	int size;

	if (!str || !envp)
		return ;
	i = 0;
	size = env_size(str);
	*envp = malloc(sizeof(char *) * (size + 1));
	if (!*envp)
		return ;
	while (i < size)
	{
		(*envp)[i] = ft_strdup(str[i]);
		i++;
	}
	(*envp)[i] = NULL;
}
