/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: batuhan <batuhan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/23 08:59:18 by bolcay            #+#    #+#             */
/*   Updated: 2025/04/16 11:52:31 by batuhan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static int	append_check(char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (str[i] == '+')
		{
			i++;
			if (str[i] && str[i] == '=')
				return (1);
			else
				return (0);
		}
		i++;
	}
	return (0);
}

static int	valid_name(char *str)
{
	int	i;
	char *temp;

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
	else if (ft_strchr(args, '-') != 0)
		return (-2);
	else if (ft_strchr(args, ' ') != 0)
		return (-1);
	else if (ft_strchr(args, 9) != 0)
		return (-1);
	else if (ft_strchr(args, 10) != 0)
		return (-1);
	else if (ft_strchr(args, 11) != 0)
		return (-1);
	else if (ft_strchr(args, 12) != 0)
		return (-1);
	else if (ft_strchr(args, 13) != 0)
		return (-1);
	else if (ft_isdigit(args[0]) != 0)
		return (-1);
	else if (args[0] == '=')
		return (-1);
	else if (valid_name(args) != 0)
		return (-1);
	return (0);
}

static int	duplicate_check_env(char *str, t_env *env)
{
	int		i;
	size_t		size;

	size = key_size(str);
	i = 0;
	while (env->envp && env->envp[i])
	{
		if (ft_strncmp(env->envp[i], str, size) == 0)
		{
			if (size != ft_strlen(env->envp[i]))
				i++;
			else
				return (0);
		}
		i++;
	}
	if (!env->envp)
		return (1);
	if (!env->envp[i])
		return (1);
	return (0);
}

static int	duplicate_check(char *args, t_env *env)
{
	int		i;
	size_t		size;

	size = key_size(args);
	i = 0;
	while (env->export && env->export[i])
	{
		if (ft_strncmp(env->export[i], args, size) == 0)
		{
			if (size != ft_strlen(env->export[i]))
				i++;
			else
				return (0);
		}
		i++;
	}
	if (!env->export)
		return (1);
	if (!env->export[i])
		return (1);
	return (0);
}

static void	duplicate_fix_env(char *str, t_env *env)
{
	int	i;
	int	size;

	i = 0;
	size = key_size(str);
	while (env->envp[i])
	{
		if (ft_strncmp(env->envp[i], str, size) == 0)
		{
			if(env->envp[i])
				free(env->envp[i]);
			env->envp[i] = ft_strdup(str);
			break ;
		}
		i++;
	}
	env->exit_code = 0;
}

static void	duplicate_fix(char *str, t_env *env)
{
	int	i;
	int	size;

	i = 0;
	size = key_size(str);
	while (env->export && env->export[i])
	{
		if (ft_strncmp(env->export[i], str, size) == 0)
		{
			if (env->export[i])
				free(env->export[i]);
			env->export[i] = ft_strdup(str);
			break ;
		}
		i++;
	}
	if (duplicate_check_env(str, env) == 0)
		duplicate_fix_env(str, env);
	else
		env->envp = update_env(env->envp, str);
	env->exit_code = 0;
}

static char	*append_organiser(char *str)
{
	int	i;
	char	*temp;
	char	*temp1;
	char	*result;

	i = 0;
	while (str[i] && str[i] != '+')
		i++;
	temp = ft_substr(str, 0, i);
	i += 1;
	temp1 = ft_substr(str, i, ft_strlen(str) - i);
	result = ft_strjoin(temp, temp1);
	free(temp);
	free(temp1);
	return (result);
}

static int	append_key_size(char *str)
{
	int i;

	i = 0;
	while (str[i])
	{
		if (str[i] == '+')
			return (i - 1);
		i++;
	}
	return (i);
}

static void	append_exp(char *str, t_env *env)
{
	int	i;
	int	size;
	char	*temp;
	char	*value;
	char	*key;

	i = 0;
	size = 0;
	while (str[size] && str[size] != '=')
		size++;
	value = ft_substr(str, size, ft_strlen(str) - size);
	size = 0;
	while (str[size] && str[size] != '+')
		size++;
	key = ft_substr(str, 0, size);
	while (env->export && env->export[i])
	{
		if (ft_strncmp(env->export[i], key, size) == 0)
		{
			if (size == append_key_size(str))
				break ;
		}
		i++;
	}
	if (env->export && env->export[i])
	{
		temp = ft_strjoin(env->export[i], value);
		free(env->export[i]);
		env->export[i] = ft_strdup(temp);
		free(temp);
		free(value);
		free(key);
	}
	else
	{
		temp = append_organiser(str);
		if (value)
			free(value);
		if (key)
			free(key);
		env->export = update_env(env->export, temp);
		free(temp);
	}
}

static void	append_env(char *str, t_env *env)
{
	int	i;
	int	size;
	char	*temp;
	char	*value;
	char	*key;

	i = 0;
	size = 0;
	while (str[size] && str[size] != '=')
		size++;
	value = ft_substr(str, size, ft_strlen(str) - size);
	size = 0;
	while (str[size] && str[size] != '+')
		size++;
	key = ft_substr(str, 0, size);
	while (env->envp && env->envp[i])
	{
		if (ft_strncmp(env->envp[i], key, size) == 0)
		{
			if (size == append_key_size(str))
				break ;
		}
		i++;
	}
	if (env->envp && env->envp[i])
	{
		temp = ft_strjoin(env->envp[i], value);
		free(env->envp[i]);
		env->envp[i] = ft_strdup(temp);
		free(temp);
		free(value);
		free(key);
	}
	else
	{
		temp = append_organiser(str);
		if (value)
			free(value);
		if (key)
			free(key);
		env->envp = update_env(env->envp, temp);
		free(temp);
	}
}

void	run_export(char **args, t_env *env)
{
	int	i;
	int	j;
	int	check;
	int	name_c;

	i = 0;
	j = 1;
	check = 0;
	name_c = 0;
	if (!args[1])
	{
		while (env->export[i])
			printf("declare -x %s\n", env->export[i++]);
		env->exit_code = 0;
		return ;
	}
	while (args[j])
	{
		name_c = name_check(args[j]);
		if (name_c == -1)
		{
			fprintf(stderr, "minishell: export: '%s': not a valid identifier\n", args[j]);
			env->exit_code = 1;
			check = 1;
		}
		else if (name_c == -2)
		{
			fprintf(stderr, "minishell: export: %s: invalid option\n", args[j]);
			env->exit_code = 2;
			check = 2;
		}
		else if (name_c == -3)
		{
			append_env(args[j], env);
			append_exp(args[j], env);
		}
		else
		{
			if (ft_strchr(args[j], '=') != 0)
			{
				if (append_check(args[j]) != 0)
				{
					if (duplicate_check(args[j], env) == 0 && duplicate_check_env(args[j], env) == 0)
					{
						duplicate_fix(args[j], env);
						duplicate_fix_env(args[j], env);
					}
					else if (duplicate_check(args[j], env) == 0)
						duplicate_fix(args[j], env);
					else
					{
						env->envp = update_env(env->envp, args[j]);
						env->export = update_env(env->export, args[j]);
					}
				}
				if (duplicate_check(args[j], env) == 0 && duplicate_check_env(args[j], env) == 0)
				{
					duplicate_fix(args[j], env);
					duplicate_fix_env(args[j], env);
				}
				else if (duplicate_check(args[j], env) == 0)
					duplicate_fix(args[j], env);
				else
				{
					env->envp = update_env(env->envp, args[j]);
					env->export = update_env(env->export, args[j]);
				}
			}
			else
				env->export = update_env(env->export, args[j]);
		}
		j++;
	}
	env->exit_code = check;
}
