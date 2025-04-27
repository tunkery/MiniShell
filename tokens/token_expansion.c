#include "../minishell.h"

char *expand_exit_status(int *i,t_env *env)
{
    char *exit_status;

    (*i)++;
    exit_status = ft_itoa(env->exit_code);
    gc_register(env->s_gc, exit_status);
    return (exit_status);
}
char *expand_quote_cond(char *line, int *i, char quote_str)
{
    char quote_status[1024];
    int quo_i = 0;

    while(line[*i] && line[*i] != quote_str)
        quote_status[quo_i++] = line[(*i)++];
    quote_status[quo_i] = '\0';
    if(line[*i] == quote_str)
        (*i)++;
    return ft_strdup(quote_status);
}

char *quote_expansion(char *line,int *i,char quote_str, t_env *env)
{
    char *quote_status;
    char *result;

    quote_status = expand_quote_cond(line,i,quote_str);
    result = ansi_c_quote(quote_status,env);
    free(quote_status);
    return result;
}

char *variable_expansion(char *line, int *i, t_env *env)
{
    char var_name[256];
    char *value;
    int j;

    j = 0;
    while(line[*i] && (ft_isalnum(line[*i]) || line[*i] == '_'))
    {
        var_name[j++] = line[(*i)++];
    }
    var_name[j] = '\0';
    value = get_path(var_name, env);
    if(!value)
    {
        value = ft_strdup("");
        gc_register(env->s_gc, value);
    }
    return (value);
}

char *expand_env(char *line, int *i, t_env *env)
{

    (*i)++;

    if(line[*i] == '?')
        return expand_exit_status(i,env);
    else if(line[*i] == '\'')
        return quote_expansion(line,i,'\'',env);

    return variable_expansion(line,i,env);

}