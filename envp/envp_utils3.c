/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   envp_utils3.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: batuhan <batuhan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/29 11:17:12 by hpehliva          #+#    #+#             */
/*   Updated: 2025/04/29 19:12:27 by batuhan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	*ft_substr_ex(char const *s, unsigned int start, size_t len)
{
	char	*new_s;
	size_t	i;
	size_t	s_len;

	if (!s)
		return (NULL);
	i = 0;
	s_len = ft_strlen(s);
	if (start > s_len)
		return (ft_strdup(""));
	if (len > s_len - start)
		len = s_len - start;
	new_s = (char *)malloc(len + 3);
	if (!new_s)
		return (NULL);
	new_s[i] = '"';
	i++;
	while (i < len)
	{
		new_s[i] = s[start + i];
		i++;
	}
	new_s[i++] = '"';
	new_s[i] = '\0';
	return (new_s);
}

char	*copy_ex_helper(char *str)
{
	int		size;
	char	*key;
	char	*value;
	char	*result;

	size = key_size(str) + 1;
	key = ft_substr(str, 0, size + 1);
	value = ft_substr_ex(str, size, ft_strlen(str) - size);
	result = ft_strjoin(key, value);
	if (key)
		free(key);
	if (value)
		free(value);
	if (!result)
		return (NULL);
	return (result);
}

void	copy_ex(char **str, char ***envp, t_env *env)
{
	int		i;
	int		size;
	char	*temp;

	if (!str || !envp)
		return ;
	i = 0;
	size = env_size(str);
	*envp = my_malloc(env->gc, sizeof(char *) * (size + 1));
	if (!*envp)
		return ;
	while (i < size)
	{
		temp = copy_ex_helper(str[i]);
		(*envp)[i] = ft_strdup(temp);
		gc_register(env->gc, (*envp)[i]);
		free(temp);
		i++;
	}
	(*envp)[size] = NULL;
}
