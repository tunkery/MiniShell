/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bolcay <bolcay@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/23 08:56:22 by bolcay            #+#    #+#             */
/*   Updated: 2025/04/23 16:41:09 by bolcay           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static int	is_n_flag(char *str)
{
	int	i;

	if (!str || str[0] != '-')
		return (0);
	i = 1;
	while (str[i])
	{
		if (str[i] != 'n')
			return (0);
		i++;
	}
	return (i > 1);
}

static void	echo_helper(char **args, int *i, int *check)
{
	while (args[*i] && is_n_flag(args[*i]))
	{
		(*check) = 1;
		(*i)++;
	}
}

void	run_echo(char **args, t_env *env)
{
	int	check;
	int	i;
	int	flag;

	if (!args[1])
	{
		printf("\n");
		env->exit_code = 0;
		return ;
	}
	check = 0;
	i = 1;
	echo_helper(args, &i, &check);
	flag = 1;
	while (args[i])
	{
		if (!flag)
			printf(" ");
		printf("%s", args[i]);
		flag = 0;
		i++;
	}
	if (check == 0)
		printf("\n");
	env->exit_code = 0;
}
