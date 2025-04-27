/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bolcay <bolcay@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/20 15:53:37 by hpehliva          #+#    #+#             */
/*   Updated: 2025/03/20 16:59:12 by bolcay           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int ft_strcmp(const char *s1, const char *s2)
{
    while (*s1 && *s2 && *s1 == *s2)
    {
        s1++;
        s2++;
    }
    return ((unsigned char)*s1 - (unsigned char)*s2);
}

char *handle_regular_char(char *result,char *line,int *i,t_env *env)
{
    char cpy[2];

    cpy[0]= line[*i];
    cpy[1]= '\0';
    result = ft_strjoin(result, cpy);
    gc_register(env->s_gc, result);
    (*i)++;
    return (result);
}

char	*get_path(char *str, t_env *env)
{
    int i;
    int len;
    char *value;

    len = ft_strlen(str);
    i = 0;
    while(env->envp[i])
    {
        if(ft_strncmp(env->envp[i], str, len) == 0 && env->envp[i][len] == '=')
        {
            value = ft_substr(env->envp[i], len+1, ft_strlen(env->envp[i])-len - 1);
            gc_register(env->s_gc, value);
            return value;
        }
        i++;
    }
    return NULL;
}

char get_escape(char c)
{
    if(c == 'n')
        return '\n';
    else if (c == 't')
        return '\t';
    else if (c == 'r')
        return '\r';
    else if (c == 'a')
        return '\a';
    else if (c == 'b')
        return '\b';
    else if (c == 'f')
        return '\f';
    else if (c == 'v')
        return '\v';
    else if(c == '\\')
        return '\\';
    else if(c == '\'')
        return '\'';
    else if(c == '"')
        return '"';
    else if(c == '$')
        return c;
    else
        return c;
}

char    *extract_word( char *line, int *i, t_env *env)
{

    char *result = ft_strdup("");
    gc_register(env->s_gc, result);
    while(line[*i] && line[*i] != ' ' && line[*i] != '|' && line[*i] != '<' && line[*i] != '>' && line[*i] != '\'' && line[*i] != '"' && line[*i] != ';')
    {
        result = handle_regular_char(result,line,i,env);

    }
    return (result);
}