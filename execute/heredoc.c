/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: batuhan <batuhan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/24 12:07:05 by hpehliva          #+#    #+#             */
/*   Updated: 2025/04/22 16:26:56 by batuhan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	*ft_strjoin_free(char *s1, char *s2)
{
	char	*result;

	result = ft_strjoin_heredoc(s1, s2);
	return (result);
}

char	*expand_command_substitution(char *line, int *index, t_env *env)
{
	char	expand_line[256];
	char	*temp;
	int		i;

	i = 0;
	(*index) += 1;
	while (line[*index] && line[*index] != ')')
		expand_line[i++] = line[(*index)++];
	expand_line[i] = '\0';
	if (line[*index] == ')')
		(*index)++;
	i = 0;
	temp = expand_env(expand_line, &i, env);
	return (temp);
}

char	*expanded_heredoc_line(char *line, t_env *env)
{
	char	*result;
	char	*temp;
	int		i;

	result = ft_strdup("");
	gc_register(env->s_gc, result);
	i = 0;
	while (line[i])
	{
		if (line[i] == '$' && line[i + 1] == '(')
		{
			temp = expand_command_substitution(line, &i, env);
			if (temp)
			{
				result = ft_strjoin_free(result, temp);
				gc_register(env->s_gc, result);
			}
		}
		else
		{
			i = expanded_heredoc_env(line, &i, &result, env);
		}
	}
	return (result);
}

char *append_heredoc_line(char*result,char *line,t_env *env,int quote_mode)
{
	if(quote_mode == 1)
	{
		result = ft_strjoin_heredoc(result,line);
		gc_register(env->s_gc, result);
		result = ft_strjoin_heredoc(result, "\n");
		gc_register(env->s_gc, result);

	}
	else
	{
		result = process_heredoc_line(line, result, env);
		gc_register(env->s_gc, result);
	}
	return result;

}


char	*handler_heredoc(char *delimiter, t_env *env, int quote_mode)
{
	char	*line;
	char	*result;

	result = ft_strdup("");
	gc_register(env->s_gc, result);
	while (1)
	{
		line = readline("> ");
		if (!line)
			break ;
		gc_register(env->s_gc, line);
		if (ft_strcmp(line, delimiter) == 0)
			break ;
		result = append_heredoc_line(result,line,env,quote_mode);
		// if(quote_mode == 1)
		// {
		// 	result = ft_strjoin_heredoc(result,line);
		// 	gc_register(env->s_gc, result);
		// 	result = ft_strjoin_heredoc(result, "\n");
		// 	gc_register(env->s_gc, result);

		// }
		// else
		// {
		// 	result = process_heredoc_line(line, result, env);
		// 	gc_register(env->s_gc, result);
		// }
	}
	return (result);
}
