/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bolcay <bolcay@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/23 09:02:44 by bolcay            #+#    #+#             */
/*   Updated: 2025/04/13 18:55:08 by bolcay           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static int	aq_exiti(const char *str, long long *out)
{
	int	i;
	int	sign;
	unsigned long long	check;
	unsigned digit;

	i = 0;
	sign = 1;
	check = 0;
	if (str[i] == '-')
	{
		sign = -1;
		i++;
	}
	else if (str[i] == '+')
		i++;
	if (ft_isdigit(str[i]) == 0)
		return (1);
	while (str[i])
	{
		if (ft_isdigit(str[i]) == 0)
			return (1);
		digit = (unsigned)(str[i] - '0');
		if (check > (ULLONG_MAX - digit) / 10)
			return (2);
		check = check * 10 + digit;
		i++;
	}
	if (sign == -1)
	{
		if (check > 9223372036854775808ULL)
			return (2);
		else if (check == 9223372036854775808ULL)
			*out = LLONG_MIN;
		else
			*out = (long long)check;
	}
	return (0);
}

void	run_exit(char **args, t_env *env)
{
	int	i;
	long long value;
	int	err;
	unsigned char status;
	(void)env;
	
	i = 0;
	while (args[i])
		i++;
	if (i == 1)
		exit(0);
	else if (i == 2)
	{
		err = aq_exiti(args[1], &value);
		if (err != 0)
			exit(255);
		else
		{
			status = (unsigned char) value;
			exit(status);
		}
	}
	else
	{
		fprintf(stderr, "exit: too many arguments\n");
	}
}
