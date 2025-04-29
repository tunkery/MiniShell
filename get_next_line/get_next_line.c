/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bolcay <bolcay@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/01 15:50:22 by bolcay            #+#    #+#             */
/*   Updated: 2025/03/18 20:19:44 by bolcay           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

static char	*ft_copy_to_temp(char *buffer, char *temp);
static char	*ft_line_copy(char **temp);
static char	*ft_read_buffer(int fd, char *temp);

char	*get_next_line(int fd)
{
	static char	*temp = NULL;
	char		*line;

	if (BUFFER_SIZE <= 0 || fd < 0)
		return (NULL);
	line = ft_line_copy(&temp);
	if (line)
		return (line);
	temp = ft_read_buffer(fd, temp);
	if (temp == NULL)
		return (free(temp), temp = NULL, NULL);
	if (ft_gnl_strchr(temp, '\n') == -1)
	{
		line = ft_gnl_strdup(temp);
		if (!line)
			return (free(temp), temp = NULL, NULL);
		free(temp);
		return (temp = NULL, line);
	}
	line = ft_line_copy(&temp);
	if (line)
		return (line);
	if (!temp)
		return (free(temp), temp = NULL, line);
	return (free(temp), temp = NULL, NULL);
}

static char	*ft_read_buffer(int fd, char *temp)
{
	char	buffer[BUFFER_SIZE + 1];
	int		i;

	while (1)
	{
		i = read(fd, buffer, BUFFER_SIZE);
		if (i == 0 && temp && temp[0] != '\0')
			return (temp);
		if (i == 0)
			break ;
		if (i == -1)
			return (free(temp), temp = NULL, NULL);
		buffer[i] = '\0';
		temp = ft_copy_to_temp(buffer, temp);
		if (!temp)
			return (NULL);
		if (ft_gnl_strchr(temp, '\n') != -1)
			return (temp);
	}
	return (temp);
}

static char	*ft_line_copy(char **temp)
{
	char	*line;
	char	*new_temp;
	int		position;

	if (!*temp)
		return (NULL);
	position = ft_gnl_strchr(*temp, '\n');
	if (position == -1)
		return (NULL);
	line = ft_gnl_substr(*temp, 0, position + 1);
	if (line && line[0] == '\0')
	{
		free(line);
		line = NULL;
	}
	new_temp = ft_gnl_substr(*temp, position + 1, ft_gnl_strlen(*temp)
			- position - 1);
	if (new_temp && new_temp[0] == '\0')
	{
		free(new_temp);
		new_temp = NULL;
	}
	free(*temp);
	*temp = new_temp;
	return (line);
}

static char	*ft_copy_to_temp(char *buffer, char *temp)
{
	char	*new_temp;

	if (temp == NULL)
	{
		temp = ft_gnl_strdup(buffer);
		if (!temp)
			return (NULL);
	}
	else
	{
		new_temp = ft_gnl_strjoin(temp, buffer, ft_gnl_strlen(buffer));
		if (!new_temp)
		{
			free(temp);
			temp = NULL;
			return (NULL);
		}
		free(temp);
		temp = new_temp;
	}
	return (temp);
}
