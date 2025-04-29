/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   envp_utils4.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hpehliva <hpehliva@student.42heilbronn.de  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/29 11:20:22 by hpehliva          #+#    #+#             */
/*   Updated: 2025/04/29 11:20:23 by hpehliva         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	*extract_shlvl_value(char *shlvl_entry)
{
	char	*value;
	size_t	slen;

	slen = ft_strlen(shlvl_entry);
	if (slen >= 9)
		value = ft_substr(shlvl_entry, 6, 3);
	else if (slen >= 8)
		value = ft_substr(shlvl_entry, 6, 2);
	else
		value = ft_substr(shlvl_entry, 6, 1);
	return (value);
}

char	*create_new_shlvl(int digit)
{
	char	*value;
	char	*temp;
	char	*temp2;

	value = ft_itoa(digit);
	temp = ft_strjoin("SHLVL=", value);
	temp2 = copy_ex_helper(temp);
	free(temp);
	if (value)
		free(value);
	return (temp2);
}

void	shell_level_ex(char ***str, int i, t_env *env)
{
	char	*value;
	char	*new_shlvl;
	int		digit;

	while ((*str)[i] && ft_strncmp((*str)[i], "SHLVL", 5) != 0)
		i++;
	value = extract_shlvl_value((*str)[i]);
	digit = ft_atoi(value);
	digit++;
	if (value)
		free(value);
	new_shlvl = create_new_shlvl(digit);
	(*str)[i] = ft_strdup(new_shlvl);
	gc_register(env->gc, (*str)[i]);
	if (new_shlvl)
		free(new_shlvl);
}

char	*create_simple_shlvl(int digit)
{
	char	*value;
	char	*temp;

	value = ft_itoa(digit);
	temp = ft_strjoin("SHLVL=", value);
	if (value)
		free(value);
	return (temp);
}

void	shell_level(char ***str, int i, t_env *env)
{
	char	*value;
	char	*new_shlvl;
	int		digit;

	while ((*str)[i] && ft_strncmp((*str)[i], "SHLVL", 5) != 0)
		i++;
	value = extract_shlvl_value((*str)[i]);
	digit = ft_atoi(value);
	digit++;
	if (value)
		free(value);
	new_shlvl = create_simple_shlvl(digit);
	(*str)[i] = ft_strdup(new_shlvl);
	gc_register(env->gc, (*str)[i]);
	if (new_shlvl)
		free(new_shlvl);
}
