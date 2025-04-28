/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_tokens3.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hpehliva <hpehliva@student.42heilbronn.de  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/28 11:33:53 by hpehliva          #+#    #+#             */
/*   Updated: 2025/04/28 11:33:54 by hpehliva         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	*handle_ansi_c(char *result, char *line, int *i, t_env *env)
{
	char	quote_status[1024];
	int		quo_i;
	char	*temp;

	(*i) += 2;
	quo_i = 0;
	while (line[*i] && line[*i] != '\'')
		quote_status[quo_i++] = line[(*i)++];
	quote_status[quo_i] = '\0';
	if (line[*i] == '\'')
		(*i)++;
	temp = ansi_c_quote(quote_status, env);
	if (temp)
	{
		result = ft_strjoin(result, temp);
		gc_register(env->s_gc, result);
	}
	return (result);
}

char	*handle_backslash(char *result, char *line, int *i, t_env *env)
{
	char	cpy[2];

	(*i)++;
	if (line[*i] == '\\' && line[*i + 1] == '$')
	{
		(*i)++;
		cpy[0] = '\\';
		cpy[1] = '\0';
		result = ft_strjoin(result, cpy);
		gc_register(env->s_gc, result);
		return (result);
	}
	cpy[0] = line[*i];
	cpy[1] = '\0';
	result = ft_strjoin(result, cpy);
	gc_register(env->s_gc, result);
	(*i)++;
	return (result);
}

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

void	process_delimiter_check(t_token *delimiter, char *line, int *i,
		t_env *env)
{
	char	*result;
	char	cpy[2];

	result = delimiter->value;
	cpy[1] = '\0';
	while (line[*i] && line[*i] != ' ' && line[*i] != '\t' && line[*i] != '|'
		&& line[*i] != '<' && line[*i] != '>' && line[*i] != ';')
	{
		cpy[0] = line[*i];
		result = ft_strjoin(result, cpy);
		gc_register(env->s_gc, result);
		delimiter->value = result;
		(*i)++;
	}
}
