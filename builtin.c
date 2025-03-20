/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bolcay <bolcay@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/10 17:38:05 by bolcay            #+#    #+#             */
/*   Updated: 2025/03/20 15:50:21 by bolcay           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	cell_exit(char **args)
{
	(void)args;
	exit(0);
}

void	cell_echo(char **args, int check)
{
	int	i;

	i = 1;
	while (args[i])
	{
		if (ft_strncmp(args[i], "-n", ft_strlen(args[i])) == 0
		&& i < 3)
			i++;
		printf("%s ", args[i]);
		i++;
	}
	if (check == 0)
		printf("\n");
}

void	cell_env(t_env *env)
{
	int i;

	i = 0;
	while (env->envp[i])
	{
		printf("%s\n", env->envp[i]);
		i++;
	}
}

void	cell_cd(const char *path)
{
	(void)path;
}

void	cell_pwd(void)
{
	char buf[BUFSIZ];

	getcwd(buf, BUFSIZ);
	printf("%s\n", buf);
}

void	cell_export(char **envp, char *key)
{
	update_env(envp, key);
}

void	cell_unset(char **envp, char *key)
{
	remove_env(envp, key);
}
