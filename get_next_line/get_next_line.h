/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bolcay <bolcay@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/01 15:51:25 by bolcay            #+#    #+#             */
/*   Updated: 2025/02/17 15:17:16 by bolcay           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GET_NEXT_LINE_H
# define GET_NEXT_LINE_H

# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 10
# endif

# include <stdio.h>
# include <unistd.h>
# include <stdlib.h>
# include <stddef.h>

char	*get_next_line(int fd);
char	*ft_gnl_strdup(const char *s);
size_t	ft_gnl_strlen(const char *str);
int		ft_gnl_strchr(const char *str, int c);
char	*ft_gnl_strjoin(char const *s1, char const *s2, size_t len);
char	*ft_gnl_substr(char const *s, unsigned int start, size_t len);
#endif