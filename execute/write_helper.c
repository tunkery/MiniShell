/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   write_helpers.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bolcay <bolcay@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/29 21:40:53 by bolcay            #+#    #+#             */
/*   Updated: 2025/04/29 21:42:11 by bolcay           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	no_file(char *str)
{
	write(2, "minishell: ", 11);
	write(2, str, ft_strlen(str));
	write(2, ": No such file or directory\n", 28);
}
