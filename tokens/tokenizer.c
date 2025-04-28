/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: batuhan <batuhan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/19 14:29:20 by hpehliva          #+#    #+#             */
/*   Updated: 2025/04/22 18:21:30 by batuhan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

t_token	*handle_heredoc_token(int *i, t_token *current, t_token **head,
		t_env *env)
{
	t_token	*token;

	token = my_malloc(env->s_gc, sizeof(t_token));
	if (!token)
		return (NULL);
	token->type = TOKEN_HEREDOC;
	token->value = ft_strdup("<<");
	gc_register(env->s_gc, token->value);
	current = add_token_list(token, head, current);
	*i += 2;
	return (current);
}

t_token	*create_delimiter_token(char *line, int *i, t_token *current,
		t_env *env)
{
	t_token	*delimiter;

	skip_whitespace(line, i);
	if (!line[*i] || line[*i] == '\n' || line[*i] == '|' || line[*i] == '<'
		|| line[*i] == '>' || line[*i] == ';')
		return (current);
	delimiter = my_malloc(env->s_gc, sizeof(t_token));
	if (!delimiter)
		return (NULL);
	delimiter->type = TOKEN_WORD;
	delimiter->value = ft_strdup("");
	gc_register(env->s_gc, delimiter->value);
	process_delimiter_check(delimiter, line, i, env);
	delimiter->prev = current;
	delimiter->next = NULL;
	current->next = delimiter;
	current = delimiter;
	return (delimiter);
}

void	process_token_type(t_token_state *state)
{
	t_token	*token;

	if (state->line[state->i] == '<' && state->line[state->i + 1] == '<')
	{
		state->current = handle_heredoc_token(&state->i, state->current,
				state->head, state->env);
		if (!state->current)
			return ;
		state->current = create_delimiter_token(state->line, &state->i,
				state->current, state->env);
	}
	else
	{
		token = handle_special_token(state->line, &state->i, state->env);
		if (!token)
			return ;
		state->current = add_token_list(token, state->head, state->current);
	}
}

void	seperated_token(char *line, t_token **head, t_env *env)
{
	t_token_state	state;

	state.line = line;
	state.i = 0;
	state.current = NULL;
	state.head = head;
	state.env = env;
	while (line[state.i])
	{
		skip_whitespace(line, &state.i);
		if (line[state.i] == '\0')
			break ;
		process_token_type(&state);
		if (!state.current)
			return ;
	}
}

t_token	*tokenizer(char *line, t_env *env)
{
	t_token	*head;

	head = NULL;
	seperated_token(line, &head, env);
	return (head);
}
