/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bolcay <bolcay@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/23 08:59:18 by bolcay            #+#    #+#             */
/*   Updated: 2025/04/23 15:00:49 by bolcay           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static int	valid_name(char *str)
{
	int		i;
	char	*temp;

	i = 0;
	if (ft_strchr(str, '=') != 0)
	{
		while (str[i] && str[i] != '=')
			i++;
		temp = ft_substr(str, 0, i);
		i = 0;
		while (temp[i])
		{
			if (ft_isalnum(temp[i]) == 0 && temp[i] != '_')
			{
				free(temp);
				return (-1);
			}
			i++;
		}
		free(temp);
	}
	else
	{
		while (str[i])
		{
			if (ft_isalnum(str[i]) == 0 && str[i] != '_')
				return (-1);
			i++;
		}
	}
	return (0);
}

static int	name_check(char *args)
{
	if (!args)
		return (-1);
	else if (append_check(args) != 0)
		return (-3);
	else if (args[0] == '-')
		return (-2);
	else if (ft_isdigit(args[0]) != 0)
		return (-1);
	else if (args[0] == '=')
		return (-1);
	else if (valid_name(args) != 0)
		return (-1);
	return (0);
}

void	run_export(char **args, t_env *env)
{
	int	i;
	int	j;
	int	check;
	int	name_c;
	// char	*key;

	i = 0;
	j = 1;
	check = 0;
	if (!args[1])
	{
		while (env->export[i])
			printf("declare -x %s\n", env->export[i++]);
		env->exit_code = 0;
		return ;
	}
	while (args[j])
	{
		// printf("%s\n", args[j]);
		name_c = name_check(args[j]);
		if (name_c != 0)
			checker(args[j], name_c, &check, env);
		else
			export1(args[j], env);
		j++;
	}
	env->exit_code = check;
}
