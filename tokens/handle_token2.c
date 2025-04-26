#include "../minishell.h"

static char *handle_ansi_c(char *result,char *line,int *i,t_env *env)
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


static char *handle_backslash(char *result,char *line,int *i,t_env *env)
{
    char cpy[2];

    // cpy[0] = line[*i];
    (*i)++;
    // if(!line[*i])
    //     return NULL;
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

static char *handle_single_quote(char *result,char *line,int *i,t_env *env)
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

static char *handle_double_quote(char *result,char *line,int *i,t_env *env)
{
    char *temp;
    temp = process_quoted(line, i, '\"', env);
    if(temp)
    {

        result = ft_strjoin(result, temp);
        gc_register(env->s_gc, result);
        return result;

    }

    return NULL;
}

static char *handle_env_var(char *result,char *line,int *i,t_env *env)
{
    char *temp;
    temp = expand_env(line, i, env);
    if(temp)
    {
        // char *old_res = result;
        result = ft_strjoin(result, temp);
        gc_register(env->s_gc, result);
        // free(old_res);
        // free(temp);
    }
    else
    {
         // char *old_res = result;
         result = ft_strjoin(result, temp);
         gc_register(env->s_gc, result);
         // free(old_res);
    }
    return result;
}

static char *handle_tilde_word(char *result,char *line,int *i,t_env *env)
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

static char *handle_regular_char(char *result,char *line,int *i,t_env *env)
{
    char cpy[2];

    cpy[0]= line[*i];
    cpy[1]= '\0';
    result = ft_strjoin(result, cpy);
    gc_register(env->s_gc, result);
    (*i)++;
    return (result);
}

static char *process_dollar(char *result,char *line,int *i,t_env *env)
{
    if (line[*i + 1] == '\'')
    {
        return handle_ansi_c(result,line,i,env);
    }
    else if(line[*i + 1] && (ft_isalnum(line[*i + 1]) || line[*i + 1] == '_' || line[*i + 1] == '?' || line[*i + 1] == '"'))
    {
        return handle_env_var(result,line,i,env);
    }
    return handle_regular_char(result,line,i,env);
}

static char *process_quotes(char *result,char *line,int *i,t_env *env)
{
    if(line[*i] == '\"')
    {
        result = handle_double_quote(result,line,i,env);
    }
    else if(line[*i] == '\'')
    {
        result = handle_single_quote(result,line,i,env);
    }
    return result;
}


static char *process_escape_chars(char *result,char *line,int *i,t_env *env)
{
    if(line[*i] == '\\')
    {
        result = handle_backslash(result,line,i,env);
    }
    else if(line[*i] == '~')
    {
        result = handle_tilde_word(result,line,i,env);
    }
    return result;
}

static char *process_special_char(char *result,char *line,int *i,t_env *env)
{
    if(line[*i] == '$')
    {
        result = process_dollar(result,line,i,env);
    }
    else if(line[*i] == '\'' || line[*i] == '\"')
    {
        result = process_quotes(result,line,i,env);
        if(!result)
            return NULL;
    }
    else if(line[*i] == '\\' || line[*i] == '~')
    {
        result = process_escape_chars(result,line,i,env);
        if(!result)
            return NULL;
    }
    else
        result = handle_regular_char(result,line,i,env);

    return result;
}

static char *process_word_token(char *result,char *line,int *i,t_env *env)
{
    while(line[*i] && (line[*i] != ' '  && line[*i] != '\t' && line[*i] != '|' && line[*i] != '>' && line[*i] != '<' && line[*i] != ';'))
    {
        result = process_special_char(result,line,i,env);
        if(!result)
            return NULL;
    }
    return result;
}

void handle_word(t_token *token, char *line, int *i, t_env *env)
{
    char *result;

    result = ft_strdup("");
    gc_register(env->s_gc, result);
    token->type = TOKEN_WORD;

    result = process_word_token(result,line,i,env);
    if(!result)
    {
        token->value = NULL;
        return;
    }

    token->value = result;
}