#include "../minishell.h"




char *process_dollar(char *result,char *line,int *i,t_env *env)
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

char *process_quotes(char *result,char *line,int *i,t_env *env)
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


char *process_escape_chars(char *result,char *line,int *i,t_env *env)
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

char *process_special_char(char *result,char *line,int *i,t_env *env)
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

char *process_word_token(char *result,char *line,int *i,t_env *env)
{
    while(line[*i] && (line[*i] != ' '  && line[*i] != '\t' && line[*i] != '|' && line[*i] != '>' && line[*i] != '<' && line[*i] != ';'))
    {
        result = process_special_char(result,line,i,env);
        if(!result)
            return NULL;
    }
    return result;
}

