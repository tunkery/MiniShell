/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bolcay <bolcay@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/01 15:51:16 by bolcay            #+#    #+#             */
/*   Updated: 2025/02/17 15:17:19 by bolcay           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

char	*ft_gnl_strdup(const char *s)
{
	size_t	i;
	size_t	j;
	char	*new_s;

	if (!s)
		return (NULL);
	i = 0;
	j = ft_gnl_strlen(s);
	new_s = (char *) malloc(j + 1);
	if (!new_s)
		return (NULL);
	while (i < j)
	{
		new_s[i] = s[i];
		i++;
	}
	new_s[i] = '\0';
	return (new_s);
}

size_t	ft_gnl_strlen(const char *str)
{
	size_t	i;

	if (!str)
		return (0);
	i = 0;
	while (str[i])
		i++;
	return (i);
}

int	ft_gnl_strchr(const char *str, int c)
{
	int	i;

	i = 0;
	while (str[i] != '\0')
	{
		if (str[i] == (char)c)
			return (i);
		i++;
	}
	return (-1);
}

char	*ft_gnl_strjoin(char const *s1, char const *s2, size_t len)
{
	size_t		i;
	size_t		j;
	char		*new_s;

	if (!s1 || !s2)
		return (NULL);
	i = 0;
	j = 0;
	new_s = malloc (ft_gnl_strlen(s1) + ft_gnl_strlen(s2) + 1);
	if (!new_s)
		return (NULL);
	while (s1[i])
	{
		new_s[i] = s1[i];
		i++;
	}
	while (s2[j] && j < len)
	{
		new_s[i + j] = s2[j];
		j++;
	}
	new_s[i + j] = '\0';
	return (new_s);
}

char	*ft_gnl_substr(char const *s, unsigned int start, size_t len)
{
	char	*new_s;
	size_t	i;
	size_t	s_len;

	if (!s)
		return (NULL);
	i = 0;
	s_len = ft_gnl_strlen(s);
	if (start >= s_len)
		return (ft_gnl_strdup(""));
	if (len > s_len - start)
		len = s_len - start;
	new_s = (char *)malloc(len + 1);
	if (!new_s)
		return (NULL);
	while (i < len)
	{
		new_s[i] = s[start + i];
		i++;
	}
	new_s[i] = '\0';
	return (new_s);
}
