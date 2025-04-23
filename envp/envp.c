/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   envp.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bolcay <bolcay@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 12:08:19 by bolcay            #+#    #+#             */
/*   Updated: 2025/04/23 17:07:52 by bolcay           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static void	shell_level_ex(char ***str, int i, t_env *env)
{
	char	*value;
	char	*temp;
	char	*temp2;
	int		digit;
	size_t	slen;

	while ((*str)[i] && ft_strncmp((*str)[i], "SHLVL", 5) != 0)
		i++;
	slen = ft_strlen((*str)[i]);
	if (slen >= 9)
		value = ft_substr((*str)[i], 6, 3);
	else if (slen >= 8)
		value = ft_substr((*str)[i], 6, 2);
	else
		value = ft_substr((*str)[i], 6, 1);
	digit = ft_atoi(value);
	free(value);
	value = ft_itoa(++digit);
	temp = ft_strjoin("SHLVL=", value);
	temp2 = copy_ex_helper(temp);
	free(temp);
	free(value);
	(*str)[i] = ft_strdup(temp2);
	gc_register(env->gc, (*str)[i]);
	free(temp2);
}

static void	shell_level(char ***str, int i, t_env *env)
{
	char	*value;
	char	*temp;
	int		digit;
	size_t	slen;

	while ((*str)[i] && ft_strncmp((*str)[i], "SHLVL", 5) != 0)
		i++;
	slen = ft_strlen((*str)[i]);
	if (slen >= 9)
		value = ft_substr((*str)[i], 6, 3);
	else if (slen >= 8)
		value = ft_substr((*str)[i], 6, 2);
	else
		value = ft_substr((*str)[i], 6, 1);
	digit = ft_atoi(value);
	if (value)
		free(value);
	value = ft_itoa(++digit);
	temp = ft_strjoin("SHLVL=", value);
	if (value)
		free(value);
	(*str)[i] = ft_strdup(temp);
	gc_register(env->gc, (*str)[i]);
	if (temp)
		free(temp);
}

void	initiate_env(t_env *env, char **envp)
{
	int	i;

	i = 0;
	copy_env(envp, &(env->envp), env);
	copy_ex(envp, &(env->export), env);
	shell_level(&(env->envp), 0, env);
	shell_level_ex(&(env->export), 0, env);
	env->exit_code = 0;
	env->save_stdin = -1;
	while (env->envp[i] && ft_strncmp(env->envp[i], "PWD", 3) != 0)
		i++;
	env->curr_pwd = ft_strdup(env->envp[i]);
	gc_register(env->gc, env->curr_pwd);
	i = 0;
	while (env->envp[i] && ft_strncmp(env->envp[i], "OLDPWD", 6) != 0)
		i++;
	if (env->envp[i])
	{
		env->old_pwd = ft_strdup(env->envp[i]);
		if (!env->envp)
			return ;
		gc_register(env->gc, env->envp[i]);
	}
}
