#include "../minishell.h"

int has_pipes(t_token *tokens)
{
    t_token *tmp = tokens;

    while (tmp && tmp->type != TOKEN_SEMIC) {
        if (tmp->type == TOKEN_PIPE)
            return 1;
        tmp = tmp->next;
    }
    return 0;
}

