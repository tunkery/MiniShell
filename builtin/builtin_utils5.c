/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_utils5.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bolcay <bolcay@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/23 15:09:59 by bolcay            #+#    #+#             */
/*   Updated: 2025/04/23 16:06:13 by bolcay           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	unset_checker(char *args, t_env *env)
{
	if (!args)
	{
		env->exit_code = 0;
		return (0);
	}
	if (args[0] == 0)
	{
		write(2, "minishell: unset: '", 19);
		write(2, args, ft_strlen(args));
		write(2, "': not a valid identifier\n", 26);
		env->exit_code = 1;
		return (0);
	}
	if (args[0] == '-')
	{
		write(2, "minishell: unset: -", 19);
		write(2, &args[1], 1);
		write(2, ": invalid option\n", 17);
		env->exit_code = 2;
		return (0);
	}
	return (-1);
}

int	unset_checker2(char *args, t_env *env)
{
	char	*temp;

	if (ft_strchr(args, ';') != 0)
	{
		temp = extract_c(args, ';');
		write(2, "Command '", 9);
		write(2, temp, ft_strlen(temp));
		write(2, "' not found\n", 12);
		free(temp);
		env->exit_code = 127;
		return (0);
	}
	else if (valid_name_unset(args) != 0)
	{
		write(2, "minishell: unset: '", 19);
		write(2, args, ft_strlen(args));
		write(2, "': not a valid identifier\n", 26);
		env->exit_code = 1;
		return (0);
	}
	return (-1);
}
