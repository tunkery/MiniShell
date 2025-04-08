/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   envp_utils2.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bolcay <bolcay@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/01 11:18:02 by bolcay            #+#    #+#             */
/*   Updated: 2025/04/01 11:50:15 by bolcay           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	*find_path(t_env *env)
{
	int	i;

	i = 0;
	while (env->envp[i] && ft_strncmp(env->envp[i], "PATH", 4) != 0)
		i++;
	if (!env->envp[i])
		return (NULL);
	return (env->envp[i]);
}
