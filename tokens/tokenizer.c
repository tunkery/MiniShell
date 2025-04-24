/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bolcay <bolcay@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/19 14:29:20 by hpehliva          #+#    #+#             */
/*   Updated: 2025/04/24 13:44:16 by bolcay           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	*handle_tilde(char *line, int *i, t_env *env)
{
	int		j;
	char	*result;
	char	*home_path;

	j = 0;
	result = NULL;
	home_path = NULL;
	((*i)++);
	while (env->envp[j] && ft_strncmp(env->envp[j], "HOME=", 5) != 0)
		j++;
	if (env->envp[j])
		home_path = ft_substr(env->envp[j], 5, ft_strlen(env->envp[j]) - 5);
	else
		env->envp[j] = "~";
	if (line[*i] == '/' || line[*i] == '\0' || line[*i] == ' ')
		result = ft_strdup(home_path);
	else
		result = ft_strdup("~");
	free(home_path);
	gc_register(env->s_gc, result);
	return (result);
}

static char	*get_path(char *str, t_env *env)
{
	int		i;
	int		len;
	char	*value;

	len = ft_strlen(str);
	i = 0;
	while (env->envp[i])
	{
		if (ft_strncmp(env->envp[i], str, len) == 0 && env->envp[i][len] == '=')
		{
			value = ft_substr(env->envp[i], len + 1,
					ft_strlen(env->envp[i]) - len - 1);
			gc_register(env->s_gc, value);
			return (value);
		}
		i++;
	}
	return (NULL);
}

char	*expand_env(char *line, int *i, t_env *env)
{
	char	*value;
	char	var_name[256];
	char	*exit_status;
	int		j;

	j = 0;
	(*i)++;
	if (line[*i] == '?')
	{
		(*i)++;
		exit_status = ft_itoa(env->exit_code);
		gc_register(env->s_gc, exit_status);
		return (exit_status);
	}
	while (line[*i] && (ft_isalnum(line[*i]) || line[*i] == '_'))
		var_name[j++] = line[(*i)++];
	var_name[j] = '\0';
	value = get_path(var_name, env);
	if (!value)
		value = ft_strdup("");
	gc_register(env->s_gc, value);
	return (value);
}

char	*process_quoted(char *line, int *i, char quote_type, t_env *env)
{
	char	*result;

	result = ft_strdup("");
	gc_register(env->s_gc, result);
	(*i)++;
	while (line[*i] && line[*i] != quote_type)
	{
		if (quote_type == '"' && line[*i] == '$' && line[*i + 1]
			&& (ft_isalnum(line[*i + 1]) || line[*i + 1] == '_'
				|| line[*i + 1] == '?'))
			process_quoted_helper(&result, line, i, env);
		else
			process_quoted_helper2(&result, line, i, env);
	}
	if (line[*i] == quote_type)
		(*i)++;
	else
		return (NULL);
	return (result);
}

t_token	*tokenizer(char *line, t_env *env)
{
	t_token	*head;

	head = NULL;
	seperated_token(line, &head, env);
	return (head);
}
