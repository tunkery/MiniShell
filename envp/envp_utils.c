/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   envp_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bolcay <bolcay@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 09:11:50 by bolcay            #+#    #+#             */
/*   Updated: 2025/04/18 15:10:53 by bolcay           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	key_size(char *str)
{
	int i;

	i = 0;
	while (str[i])
	{
		if (str[i] == '=')
			return (i - 1);
		i++;
	}
	return (i);
}

int	value_size(char *str)
{
	int i;
	int j;

	i = 0;
	j = 0;
	while (str[i] != '=')
		i++;
	while (str[i + j])
		j++;
	return (j);
}

int	env_size(char **envp)
{
	int i;

	i = 0;
	while (envp[i])
		i++;
	return (i);
}

void copy_env(char **str, char ***envp)
{
	int i;
	int size;

	if (!str || !envp)
		return ;
	i = 0;
	size = env_size(str);
	*envp = malloc(sizeof(char *) * (size + 1));
	if (!*envp)
		return ;
	while (i < size)
	{
		(*envp)[i] = ft_strdup(str[i]);
		i++;
	}
	(*envp)[i] = NULL;
}

static char	*ft_substr_ex(char const *s, unsigned int start, size_t len)
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
	if (!result)
		return (NULL);
	if (key)
		free(key);
	if (value)
		free(value);
	return (result);
}

void	copy_ex(char **str, char ***envp)
{
	int		i;
	int		size;
	char	*temp;
	
	if (!str || !envp)
		return ;
	i = 0;
	size = env_size(str);
	*envp = malloc(sizeof(char *) * (size + 1));
	if (!*envp)
		return ;
	while (i < size)
	{
		temp = copy_ex_helper(str[i]);
		(*envp)[i] = ft_strdup(temp);
		if (temp)
			free(temp);
		i++;
	}
	(*envp)[i] = NULL;
}
