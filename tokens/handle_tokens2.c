#include "../minishell.h"

char *handle_single_quote(char *result,char *line,int *i,t_env *env)
{
    char *temp;
    temp = process_quoted(line, i, '\'', env);
    if(temp)
    {
        result = ft_strjoin(result, temp);
        gc_register(env->s_gc, result);
        return result;
    }
    return NULL;
}

char *handle_double_quote(char *result,char *line,int *i,t_env *env)
{
    char *temp;
    temp = process_quoted(line, i, '"', env);
    if(temp)
    {
        result = ft_strjoin(result, temp);
        gc_register(env->s_gc, result);
        return result;
    }

    return NULL;
}

char *handle_env_var(char *result,char *line,int *i,t_env *env)
{
    char *temp;
    temp = expand_env(line, i, env);
    if(temp)
    {
        result = ft_strjoin(result, temp);
        gc_register(env->s_gc, result);
    }
    else
    {
         result = ft_strjoin(result, temp);
         gc_register(env->s_gc, result);
    }
    return result;
}

char *handle_tilde_word(char *result,char *line,int *i,t_env *env)
{
    char *temp;
    char cpy[2];
    if(line[*i + 1] == '/' || line[*i + 1] == '\0' || line[*i + 1] == ' ' || line[*i + 1] == '\t')
    {
        temp = handle_tilde(line, i, env);
        if(temp)
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
    return result;
}

