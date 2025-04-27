#include "../minishell.h"

char *handle_ansi_c(char *result,char *line,int *i,t_env *env)
{
    char quote_status[1024];
    int quo_i;
    char *temp;
    
    (*i) += 2;
    quo_i = 0;
    while(line[*i] && line[*i] != '\'')
        quote_status[quo_i++] = line[(*i)++];
    quote_status[quo_i] = '\0';
    if(line[*i] == '\'')
        (*i)++;
    temp = ansi_c_quote(quote_status,env);
    if(temp)
    {
        result = ft_strjoin(result,temp);
        gc_register(env->s_gc,result);
    }
    return result;
}


char *handle_backslash(char *result,char *line,int *i,t_env *env)
{
    char cpy[2];

    (*i)++;
    if(line[*i] == '\\' && line[*i + 1] == '$')
    {
        (*i)++;
        cpy[0] = '\\';
        cpy[1] = '\0';
        result = ft_strjoin(result, cpy);
        gc_register(env->s_gc, result);
        return result;
    }
    cpy[0] = line[*i];
    cpy[1] = '\0';
    result = ft_strjoin(result, cpy);
    gc_register(env->s_gc, result);
    (*i)++;
    return result;
}


char  *handle_tilde(char *line, int *i, t_env *env)
{
    int j = 0;
    char *result = NULL;
    char *home_path = NULL;

    ((*i)++); // skip the tilde
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

}
