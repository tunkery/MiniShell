/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bolcay <bolcay@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/23 08:56:22 by bolcay            #+#    #+#             */
/*   Updated: 2025/03/23 13:46:05 by bolcay           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	run_echo(char **args)
{
	int	check;
	int	i;

	check = 0;
	i = 1;
	if (!args[1])
	{
		printf("\n");
		return ;
	}
	else if (ft_strncmp(args[1], "-n", ft_strlen(args[1])) == 0
			&& !args[2])
			return ;
	if (ft_strncmp(args[1], "-n", ft_strlen(args[1])) == 0)
		check++;
	while (args[i])
	{
		if (ft_strncmp(args[i], "-n", ft_strlen(args[i])) == 0
		&& i < 3)
			i++;
		if (args[i + 1])
			printf("%s ", args[i]);
		else
			printf("%s", args[i]);
		i++;
	}
	if (check == 0)
		printf("\n");
}
