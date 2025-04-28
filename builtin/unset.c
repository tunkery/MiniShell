/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: batuhan <batuhan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/23 09:00:31 by bolcay            #+#    #+#             */
/*   Updated: 2025/04/22 14:57:06 by batuhan          ###   ########.fr       */
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

// int	valid_name_unset(char *str)
// {
// 	int	i;

// 	if (!str[0] || ft_isalnum(str[0]) != 0)
// 		return (-1);
// 	i = 0;
// 	while (str[i])
// 	{
// 		if (ft_isalnum(str[i]) == 0 && str[i] != '_')
// 			return (-1);
// 		i++;
// 	}
// 	return (0);
// }

// // static int	get_path(char *str, t_env *env)
// // {
// // 	int		j;
// // 	int		size;
// // 	char	*key;

// // 	j = 0;
// // 	key = ft_substr(str, 0, key_size(str));
// // 	// if (!key)
// // 	// 	return (0);
// // 	size = ft_strlen(key);
// // 	while (env->envp && env->envp[j])
// // 	{
// // 		if (ft_strncmp(env->envp[j], key, size) == 0)
// // 		{
// // 			free(key);
// // 			return (1);
// // 		}
// // 		j++;
// // 	}
// // 	free(key);
// // 	return (0);
// // }

// int	hey(char *str, char c)
// {
// 	int	i;

// 	i = 0;
// 	while (str[i])
// 	{
// 		if (str[i] == c)
// 			return (i + 1);
// 		i++;
// 	}
// 	return (-1);
// }

// char	*extract_c(char *str, char c)
// {
// 	int	j;

// 	j = hey(str, c);
// 	if (j == -1)
// 		return (NULL);
// 	return (ft_substr(str, j, ft_strlen(str) - j));
// }

// void	run_unset(char **args, t_env *env)
// {
// 	int	i;
// 	char	*temp;

// 	if (!args[1])
// 	{
// 		env->exit_code = 0;
// 		return ;
// 	}
// 	if (args[1][0] == 0)
// 	{
// 		// fprintf(stderr, "minishell: unset: '%s': not a valid identifier\n", args[1]);
// 		env->exit_code = 1;
// 		return ;
// 	}
// 	if (args[1][0] == '-')
// 	{
// 		fprintf(stderr, "minishell: unset: -%c: invalid option\n", args[1][1]);
// 		env->exit_code = 2;
// 		return ;
// 	}
// 	else if (ft_strchr(args[1], ';') != 0)
// 	{
// 		temp = extract_c(args[1], ';');
// 		fprintf(stderr, "Command '%s' not found\n", temp);
// 		free(temp);
// 		env->exit_code = 127;
// 		return ;
// 	}
// 	else if (valid_name_unset(args[1]) != 0)
// 	{
// 		// fprintf(stderr, "minishell: unset: '%s': not a valid identifier\n", args[1]);
// 		env->exit_code = 1;
// 		return ;
// 	}
// 	if (args[1][0] == '=')
// 	{
// 		// fprintf(stderr, "minishell: unset: '%s': not a valid identifier\n", args[1]);
// 		env->exit_code = 1;
// 		return ;
// 	}
// 	// if (get_path(args[1], env) == 0)
// 	// {
// 	// 	env->exit_code = 1;
// 	// 	return ;
// 	// }
// 	i = 1;
// 	while (args[i])
// 	{
// 		env->envp = remove_env(env->envp, args[i], env);
// 		env->export = remove_env(env->export, args[i], env);
// 		i++;
// 	}
// 	env->exit_code = 0;
// }
