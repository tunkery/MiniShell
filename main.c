/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bolcay <bolcay@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 10:09:58 by bolcay            #+#    #+#             */
/*   Updated: 2025/03/17 12:20:47 by bolcay           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int main(int ac, char **av, char **envp)
{
	int i;
	(void)av;
	(void)envp;

	if (ac != 1)
	{
		printf("Don't give any arguments!\n");
		return (0);
	}
	i = 0;
	return (0);
}
