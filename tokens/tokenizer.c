/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bolcay <bolcay@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/19 14:29:20 by hpehliva          #+#    #+#             */
/*   Updated: 2025/04/23 18:03:25 by bolcay           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	*handle_tilde(char *line, int *i, t_env *env)
{
<<<<<<< HEAD
	int		j;
	char	*result;
	char	*home_path;

	j = 0;
	result = NULL;
	home_path = NULL;
	((*i)++);
	while (env->envp[j] && ft_strncmp(env->envp[j], "HOME=", 5) != 0)
		j++;
	if (env->envp[j])
		home_path = ft_substr(env->envp[j], 5, ft_strlen(env->envp[j]) - 5);
	else
		env->envp[j] = "~";
	if (line[*i] == '/' || line[*i] == '\0' || line[*i] == ' ')
		result = ft_strdup(home_path);
	else
		result = ft_strdup("~");
	free(home_path);
	gc_register(env->s_gc, result);
	return (result);
=======
    int j = 0;
    char *result = NULL;
    char *home_path = NULL;

    ((*i)++); 
    while(env->envp[j] && ft_strncmp(env->envp[j], "HOME=", 5) != 0)
        j++;
    if(env->envp[j])
        home_path = ft_substr(env->envp[j], 5, ft_strlen(env->envp[j]) - 5);
    else
        env->envp[j] = "~";
    
    if(line[*i] == '/' || line[*i] == '\0' || line[*i] == ' ')
        result = ft_strdup(home_path);
    else
        result = ft_strdup("~");
    free(home_path);
    gc_register(env->s_gc, result);
    return(result);

>>>>>>> 415df6da96ac5111b9a697d0b5a27e7339f17f68
}

static char	*get_path(char *str, t_env *env)
{
	int		i;
	int		len;
	char	*value;

	len = ft_strlen(str);
	i = 0;
	while (env->envp[i])
	{
		if (ft_strncmp(env->envp[i], str, len) == 0 && env->envp[i][len] == '=')
		{
			value = ft_substr(env->envp[i], len + 1,
					ft_strlen(env->envp[i]) - len - 1);
			gc_register(env->s_gc, value);
			return (value);
		}
		i++;
	}
	return (NULL);
}

char	*expand_env(char *line, int *i, t_env *env)
{
<<<<<<< HEAD
	char	*value;
	char	var_name[256];
	char	*exit_status;
	int		j;

	j = 0;
	(*i)++;
	if (line[*i] == '?')
	{
		(*i)++;
		exit_status = ft_itoa(env->exit_code);
		gc_register(env->s_gc, exit_status);
		return (exit_status);
	}
	while (line[*i] && (ft_isalnum(line[*i]) || line[*i] == '_'))
	{
		var_name[j++] = line[(*i)++];
	}
	var_name[j] = '\0';
	value = get_path(var_name, env);
	if (value)
		return (value);
	else
	{
		value = ft_strdup("");
	}
	gc_register(env->s_gc, value);
	return (value);
=======
    char *value;
    char var_name[256];
    char *exit_status;
    int j;

    j = 0;
    (*i)++; 
    if(line[*i] == '?')
    {
        (*i)++;
        exit_status = ft_itoa(env->exit_code);
        gc_register(env->s_gc, exit_status);
        return (exit_status);
    }
    while(line[*i] && (ft_isalnum(line[*i]) || line[*i] == '_'))
    {
        var_name[j++] = line[(*i)++];
    }
    var_name[j] = '\0';
    value = get_path(var_name, env);
    if(value)
    {
        gc_register(env->s_gc, value);
        return (value);
    }
    else
    {
        value = ft_strdup("");
        
    }
    gc_register(env->s_gc, value);
    return (value);
>>>>>>> 415df6da96ac5111b9a697d0b5a27e7339f17f68
}

char	*process_quoted(char *line, int *i, char quote_type, t_env *env)
{
<<<<<<< HEAD
	char	*result;
	char	*temp;
	char	cpy[2];

	result = ft_strdup("");
	gc_register(env->s_gc, result);
	(*i)++;
	while (line[*i] && line[*i] != quote_type)
	{
		if (quote_type == '"' && line[*i] == '$' && line[*i + 1]
			&& (ft_isalnum(line[*i + 1]) || line[*i + 1] == '_'
				|| line[*i + 1] == '?'))
		{
			temp = expand_env(line, i, env);
			if (temp)
			{
				result = ft_strjoin(result, temp);
				gc_register(env->s_gc, result);
			}
		}
		else
		{
			cpy[0] = line[*i];
			cpy[1] = '\0';
			result = ft_strjoin(result, cpy);
			gc_register(env->s_gc, result);
			(*i)++;
		}
	}
	if (line[*i] == quote_type)
		(*i)++;
	else
		return (NULL);
	return (result);
=======
    char *result = ft_strdup("");
    char *temp;

    gc_register(env->s_gc, result);
    (*i)++;
    while(line[*i] && line[*i] != quote_type)
    {
        if(quote_type == '"' && line[*i] == '$' && line[*i + 1] && (ft_isalnum(line[*i + 1]) || line[*i + 1] == '_' || line[*i + 1] == '?'))
        {
            temp = expand_env(line, i, env);
            if(temp)
            {
                result = ft_strjoin(result, temp);
                gc_register(env->s_gc, result);
            }
        }
        else
        {
            char cpy[2] = {line[*i], '\0'};
            result = ft_strjoin(result, cpy);
            gc_register(env->s_gc, result);
            (*i)++;
        }
    }
    if(line[*i] == quote_type)
        (*i)++;
    else
        return (NULL);
    return (result);
>>>>>>> 415df6da96ac5111b9a697d0b5a27e7339f17f68
}

char	*extract_word( char *line, int *i, t_env *env)
{
<<<<<<< HEAD
	char	*result;
	char	cpy[2];

	result = ft_strdup("");
	gc_register(env->s_gc, result);
	while (line[*i] && line[*i] != ' ' && line[*i] != '|' && line[*i] != '<' && line[*i] != '>' && line[*i] != '\'' && line[*i] != '"' && line[*i] != ';')
	{
		cpy[0] = line[*i];
		cpy[1] = '\0';
		result = ft_strjoin(result, cpy);
		gc_register(env->s_gc, result);
		(*i)++;
	}
	return (result);
=======
    char *result = ft_strdup("");

    gc_register(env->s_gc, result);
    while(line[*i] && line[*i] != ' ' && line[*i] != '|' && line[*i] != '<' && line[*i] != '>' && line[*i] != '\'' && line[*i] != '"' && line[*i] != ';')
    {
            char cpy[2] = {line[*i], '\0'};
            result = ft_strjoin(result, cpy);
            gc_register(env->s_gc, result);
            (*i)++;

    }
    return (result);
>>>>>>> 415df6da96ac5111b9a697d0b5a27e7339f17f68
}

t_token	*handle_special_token(char *line, int *i, t_env *env)
{
<<<<<<< HEAD
	t_token	*token;

	token = my_malloc(env->s_gc, sizeof(t_token));
	if (!token)
		return (NULL);
	token->next = NULL;
	token->prev = NULL;
	if (line[*i] == '|')
		handle_pipe(token, i, env);
	else if (line[*i] == '<')
		handle_redirect_in(token, line, i, env);
	else if (line[*i] == '>')
		handle_redirect_out(token, line, i, env);
	else if (line[*i] == ';')
		handle_semic(token, i, env);
	else
		handle_word(token, line, i, env);
	if (!token->value)
		return (NULL);
	token->next = NULL;
	return (token);
=======
    t_token *token;

    token = my_malloc(env->s_gc, sizeof(t_token));
    if(!token)
        return NULL;
    token->next = NULL;
    token->prev = NULL;
    if(line[*i] == '|')
        handle_pipe(token,i, env);
    else if(line[*i] == '<')
        handle_redirect_in(token, line, i, env);
    else if(line[*i] == '>')
        handle_redirect_out(token, line, i, env);
    else if(line[*i] == ';')
        handle_semic(token, i, env);
    else
        handle_word(token, line, i, env);
    if(!token->value)
        return NULL;
    token->next = NULL;
    return token;    
>>>>>>> 415df6da96ac5111b9a697d0b5a27e7339f17f68
}

void	seperated_token(char *line, t_token **head, t_env *env)
{
<<<<<<< HEAD
	t_token	*current;
	t_token	*token;
	int		i;

	i = 0;
	current = NULL;
	while (line[i])
	{
		while (line[i] && (line[i] == ' ' || line[i] == '\t'))
			i++;
		if (line[i] == '\0')
			break ;
		token = handle_special_token(line, &i, env);
		if (!token)
			return ;
		token->prev = current;
		if (!*head)
			*head = token;
		else
			current->next = token;
		current = token;
	}
=======
    t_token *current = NULL;
    t_token *token;
    int i = 0;

    while(line[i])
    {
        while(line[i] && (line[i] == ' ' || line[i] == '\t'))
            i++;
        if(line[i] == '\0')
            break;
        token = handle_special_token(line, &i, env);
        if(!token)
            return;
        token->prev = current;
        if(!*head)
            *head = token;
        else
            current->next = token;
        current = token;
    }
>>>>>>> 415df6da96ac5111b9a697d0b5a27e7339f17f68
}

t_token	*tokenizer(char *line, t_env *env)
{
<<<<<<< HEAD
	t_token	*head;

	head = NULL;
	seperated_token(line, &head, env);
	return (head);
=======
    t_token *head = NULL;

    seperated_token(line, &head, env);
    return (head);
>>>>>>> 415df6da96ac5111b9a697d0b5a27e7339f17f68
}
