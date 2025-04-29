/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bolcay <bolcay@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/23 09:02:44 by bolcay            #+#    #+#             */
/*   Updated: 2025/04/29 21:57:07 by bolcay           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static void	free_both(t_env *env)
{
	gc_free_all(env->s_gc);
	gc_free_all(env->gc);
}

static void	write_and_exit(char **args, t_env *env)
{
	write(2, "exit\n", 5);
	write(2, "minishell: exit: ", 17);
	write(2, args[1], ft_strlen(args[1]));
	write(2, ": numeric argument required\n", 28);
	gc_free_all(env->s_gc);
	gc_free_all(env->gc);
	exit(2);
}

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
	int				code;

	if (!args[1])
	{
		free_both(env);
		exit(0);
	}
	code = exit_helper2(args, &i);
	err = aq_exiti(args[1], &value, i);
	if (code == 1)
	{
		write(2, "exit: too many arguments\n", 25);
		env->exit_code = 1;
		return ;
	}
	else if (code == 2 || err == 2)
		write_and_exit(args, env);
	status = (unsigned char)value;
	free_both(env);
	exit(status);
}
