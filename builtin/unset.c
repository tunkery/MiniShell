/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bolcay <bolcay@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/23 09:00:31 by bolcay            #+#    #+#             */
/*   Updated: 2025/04/23 15:12:13 by bolcay           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	valid_name_unset(char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (ft_isalnum(str[i]) == 0 && str[i] != '_')
			return (-1);
		i++;
	}
	return (0);
}

int	hey(char *str, char c)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (str[i] == c)
			return (i + 1);
		i++;
	}
	return (-1);
}

char	*extract_c(char *str, char c)
{
	int	j;

	j = hey(str, c);
	if (j == -1)
		return (NULL);
	return (ft_substr(str, j, ft_strlen(str) - j));
}

void	run_unset(char **args, t_env *env)
{
	int	i;
	int	check;

	check = unset_checker(args[1], env);
	if (check != 0)
		check = unset_checker2(args[1], env);
	i = 1;
	if (check != 0)
	{
		while (args[i])
		{
			env->envp = remove_env(env->envp, args[i], env);
			env->export = remove_env(env->export, args[i], env);
			i++;
		}
		env->exit_code = 0;
	}
}
