/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: batuhan <batuhan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 10:09:58 by bolcay            #+#    #+#             */
/*   Updated: 2025/04/29 19:38:31 by batuhan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*user_input(void)
{
	char	*line;
	char	*trimmer;

	if (isatty(STDIN_FILENO))
	{
		line = readline(CYAN "minishell> " RESET);
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

static int	setup_env(t_env **env, char **envp)
{
	t_gc	*gc;

	gc = gc_new();
	if (!gc)
		return (0);
	*env = my_malloc(gc, sizeof(t_env));
	if (!*env)
	{
		gc_free_all(gc);
		return (0);
	}
	(*env)->gc = gc;
	initiate_env(*env, envp);
	return (1);
}

static int	process_comd(t_env *env, t_gc *s_gc)
{
	t_token	*tokens;
	char	*line;

	s_gc = gc_new();
	if (!s_gc)
		return (0);
	env->s_gc = s_gc;
	signal_mode_read();
	line = user_input();
	if (!line)
	{
		gc_free_all(env->s_gc);
		return (0);
	}
	gc_register(env->s_gc, line);
	tokens = tokenizer(line, env);
	if (!tokens)
	{
		gc_free_all(env->s_gc);
		return (1);
	}
	signal_mode_command();
	cell_launch(tokens, env);
	gc_free_all(env->s_gc);
	return (1);
}

int	main(int ac, char **av, char **envp)
{
	t_env	*env;
	t_gc	*s_gc;

	env = NULL;
	s_gc = NULL;
	(void)av;
	if (ac > 1)
	{
		printf(RED "Don't give any arguments!\n" RESET);
		return (0);
	}
	if (!setup_env(&env, envp))
	{
		return (1);
	}
	while (1)
	{
		if (!process_comd(env, s_gc))
			break ;
	}
	gc_free_all(env->gc);
	return (0);
}
