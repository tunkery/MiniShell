/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hpehliva <hpehliva@student.42heilbronn.de  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/13 16:02:56 by hpehliva          #+#    #+#             */
/*   Updated: 2025/04/13 16:02:58 by hpehliva         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"


// Helper function to create args array from tokens
char **create_args_from_tokens(t_token *start, t_token *end)
{

    if(!start)
        return NULL;
    int count = count_args_seg(start,end);
    return args_from_token_alloc(start,end,count);
}

// I could compile with stardart handler with this but i have to check it more!! TODO!
void handle_standard_redirec(t_token **curr, int *in_fd, int *out_fd)
{
    if ((*curr)->type == TOKEN_REDIRECT_OUT) {
        *curr = (*curr)->next;
        if (*curr && (*curr)->type == TOKEN_WORD) {
            if (*out_fd != STDOUT_FILENO)
                close(*out_fd);
            *out_fd = open((*curr)->value, O_WRONLY | O_CREAT | O_TRUNC, 0644);
            if (*out_fd < 0) {
                perror("open failed");
                exit(EXIT_FAILURE);
            }
        }
    } else if ((*curr)->type == TOKEN_REDIRECT_APPEND) {
        *curr = (*curr)->next;
        if (*curr && (*curr)->type == TOKEN_WORD) {
            if (*out_fd != STDOUT_FILENO)
                close(*out_fd);
            *out_fd = open((*curr)->value, O_WRONLY | O_CREAT | O_APPEND, 0644);
            if (*out_fd < 0) {
                perror("open failed");
                exit(EXIT_FAILURE);
            }
        }
    } else if ((*curr)->type == TOKEN_REDIRECT_IN) {
        *curr = (*curr)->next;
        if (*curr && (*curr)->type == TOKEN_WORD) {
            if (*in_fd != STDIN_FILENO)
                close(*in_fd);
            *in_fd = open((*curr)->value, O_RDONLY);
            if (*in_fd < 0) {
                perror("open failed");
                exit(EXIT_FAILURE);
            }
        }
    }

}

void handle_heredoc_redirec(t_token **curr, int *in_fd,t_env *env)
{
    char *heredoc_input = NULL;
    *curr = (*curr)->next;
    if(*curr && (*curr)->type == TOKEN_WORD)
    {
        if (*in_fd != STDIN_FILENO)
            close(*in_fd);
        
        int pipe_fd[2];
        if (pipe(pipe_fd) == -1) {
            perror("pipe failed");
            exit(EXIT_FAILURE);
        }
        
        heredoc_input = handler_heredoc((*curr)->value, env);
        write(pipe_fd[1], heredoc_input, ft_strlen(heredoc_input));
        close(pipe_fd[1]);
        free(heredoc_input);
        
        *in_fd = pipe_fd[0];
    }
}


// Function to apply redirections within a piped command
void apply_redirections(t_token *start, t_token *end, int *in_fd, int *out_fd, t_env *env)
{
    t_token *current = start;
    // char *heredoc_input = NULL;
    
    while (current && current != end && current->type != TOKEN_PIPE) 
    {
        if(current->type == TOKEN_REDIRECT_OUT ||
            current->type == TOKEN_REDIRECT_APPEND ||
            current->type == TOKEN_REDIRECT_IN)
            handle_standard_redirec(&current,in_fd,out_fd);
        else if(current->type == TOKEN_HEREDOC)
            handle_heredoc_redirec(&current,in_fd,env);

        if (current)
            current = current->next;
    }
}
