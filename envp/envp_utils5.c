/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   envp_utils5.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hpehliva <hpehliva@student.42heilbronn.de  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/29 11:23:59 by hpehliva          #+#    #+#             */
/*   Updated: 2025/04/29 11:24:00 by hpehliva         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	setup_path_variable(t_env *env)
{
	env->path = malloc(sizeof(char **) * 2);
	if (!env->path)
		return ;
	gc_register(env->gc, env->path);
	env->path1 = getenv("PATH");
}

void	initiate_env(t_env *env, char **envp)
{
	copy_env(envp, &(env->envp), env);
	copy_ex(envp, &(env->export), env);
	shell_level(&(env->envp), 0, env);
	shell_level_ex(&(env->export), 0, env);
	env->exit_code = 0;
	env->save_stdin = -1;
	setup_pwd_variables(env);
	setup_path_variable(env);
}

char	*allocate_substr(size_t len, t_env *env)
{
	char	*new_s;

	new_s = (char *)malloc(len + 2);
	if (!new_s)
		return (NULL);
	gc_register(env->s_gc, new_s);
	return (new_s);
}

char	*handle_empty_substr(t_env *env)
{
	char	*new_s;

	new_s = ft_gnl_strdup("");
	gc_register(env->s_gc, new_s);
	return (new_s);
}
