/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer_utils2.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hpehliva <hpehliva@student.42heilbronn.de  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/28 12:08:18 by hpehliva          #+#    #+#             */
/*   Updated: 2025/04/28 12:08:19 by hpehliva         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	*regular_char_decrease(char *result, char *line, int *i, t_env *env)
{
	char	cpy[2];

	cpy[0] = line[*i];
	cpy[1] = '\0';
	result = ft_strjoin(result, cpy);
	gc_register(env->s_gc, result);
	return (result);
}

char	*handle_quote_var(char *line, int *i, char *result, t_env *env)
{
	char	*temp;

	temp = expand_env(line, i, env);
	if (temp)
	{
		result = ft_strjoin(result, temp);
		gc_register(env->s_gc, result);
	}
	return (result);
}
