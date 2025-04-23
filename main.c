/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bolcay <bolcay@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 10:09:58 by bolcay            #+#    #+#             */
/*   Updated: 2025/04/23 17:31:47 by bolcay           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*user_input(void)
{
	char	*line;
	char	*trimmer;

	if (isatty(STDIN_FILENO))
	{
		line = readline(CYAN"minishell> "RESET);
		if (!line)
			return (NULL);
		if (*line)
			add_history(line);
		return (line);
	}
	else
	{
		line = get_next_line(STDIN_FILENO);
		if (!line)
			return (NULL);
		trimmer = ft_strtrim(line, "\n");
		free(line);
	}
	return (trimmer);
}

int	main(int ac, char **av, char **envp)
{
	char	*line;
	t_env	*env;
	t_token	*tokens;
	t_gc	*gc;
	t_gc	*s_gc;

	(void)av;
	if (ac != 1)
	{
		printf("Don't give any arguments!\n");
		return (0);
	}
	gc = gc_new();
	if (!gc)
		return (1);
	env = my_malloc(gc, sizeof(t_env));
	if (!env)
		return (1);
	env->gc = gc;
	initiate_env(env, envp);
	while (1)
	{
		s_gc = gc_new();
		if (!s_gc)
			break ;
		env->s_gc = s_gc;
		signal_mode_read();
		line = user_input();
		if (!line)
		{
			gc_free_all(env->s_gc);
			gc_free_all(env->gc);
			return (0);
		}
		gc_register(env->s_gc, line);
		tokens = tokenizer(line, env);
		if (!tokens)
		{
			gc_free_all(env->s_gc);
			continue ;
		}
		signal_mode_command();
		cell_launch(tokens, env);
		gc_free_all(env->s_gc);
	}
	gc_free_all(env->gc);
	return (0);
}
