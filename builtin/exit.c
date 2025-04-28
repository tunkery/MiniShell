/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: batuhan <batuhan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/23 09:02:44 by bolcay            #+#    #+#             */
/*   Updated: 2025/04/16 10:33:09 by batuhan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static int	aq_exiti(const char *str, long long *out, int i)
{
	int					sign;
	unsigned long long	check;

	exit_helper1(&sign, str, &i, &check);
	if (ft_isdigit(str[i]) == 0)
		return (1);
	i = exit_helper(str, i, &check);
	if (i == 1 || i == 2)
		return (i);
	if (sign < 0)
	{
		if (check > 9223372036854775808ULL)
			return (2);
		else if (check == 9223372036854775808ULL)
			*out = LLONG_MIN;
		else
			*out = (long long)check * sign;
	}
	else
	{
		if (check > 9223372036854775807ULL)
			return (2);
		*out = (long long)check;
	}
	return (0);
}

void	run_exit(char **args, t_env *env)
{
	int				i;
	long long		value;
	int				err;
	unsigned char	status;

	(void)env;
	if (!args[1])
		exit(0);
	if (exit_helper2(args, &i) != -1)
		exit(exit_helper2(args, &i));
	if (i == 2)
	{
		err = aq_exiti(args[1], &value, 0);
		if (err != 0)
		{
			write(2, "exit: ", 6);
			write(2, args[1], ft_strlen(args[1]));
			write(2, ": numeric argument required\n", 28);
			exit(2);
		}
		status = (unsigned char)value;
		exit(status);
	}
	else
		write(2, "exit: too many arguments\n", 25);
}
