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

// Helper function to check if tokens contain pipe
int count_pipe_seg(t_token * tokens)
{
    t_token *tmp = tokens;
    int count = 0;

    while (tmp && tmp->type != TOKEN_SEMIC) {
        if (tmp->type == TOKEN_PIPE)
            count++;
        tmp = tmp->next;
    }
    count++;

    return count;

}

t_token **seg_alloc(t_token *tokens, int seg_count)
{
    t_token **segments = malloc(sizeof(t_token *) * (seg_count + 1));
    if (!segments)
        return NULL;
    
    // Find start of each segment
    t_token *tmp = tokens;
    int i = 0;
    segments[i++] = tmp; // First segment starts
    
    while (tmp && tmp->type != TOKEN_SEMIC) {
        if (tmp->type == TOKEN_PIPE) {
            segments[i++] = tmp->next; // Next segment starts after pipe
        }
        tmp = tmp->next;
    }
    
    segments[i] = NULL; // End
    
    return segments;
}

t_token **find_pipe_seg(t_token *tokens, int *seg_count)
{
    // t_token *tmp = tokens;
    // int count = 0;
    
    // // Count segments (commands separated by pipes)
    // while (tmp && tmp->type != TOKEN_SEMIC) {
    //     if (tmp->type == TOKEN_PIPE)
    //         count++;
    //     tmp = tmp->next;
    // }
    // count++; // Add one for the last segment
    *seg_count = count_pipe_seg(tokens);


    // Allocate array for segment pointers
    // t_token **segments = malloc(sizeof(t_token *) * (count + 1));
    // if (!segments)
    //     return NULL;
    
    // // Find start of each segment
    // tmp = tokens;
    // int i = 0;
    // segments[i++] = tmp; // First segment starts at the beginning
    
    // while (tmp && tmp->type != TOKEN_SEMIC) {
    //     if (tmp->type == TOKEN_PIPE) {
    //         segments[i++] = tmp->next; // Next segment starts after pipe
    //     }
    //     tmp = tmp->next;
    // }
    
    // segments[i] = NULL; // End marker
    // *seg_count = count;
    
    // return segments;
    return seg_alloc(tokens,*seg_count);
}

int count_args_seg(t_token *start,t_token *end)
{
    int count = 0;
    t_token *tmp = start;
    
     // Count the number of word tokens that aren't redirection targets
    while (tmp && tmp != end && tmp->type != TOKEN_PIPE) {
        if (tmp->type == TOKEN_WORD && 
            !(tmp->prev && (tmp->prev->type == TOKEN_REDIRECT_IN || 
                           tmp->prev->type == TOKEN_REDIRECT_OUT || 
                           tmp->prev->type == TOKEN_REDIRECT_APPEND || 
                           tmp->prev->type == TOKEN_HEREDOC))) {
            count++;
        }
        tmp = tmp->next;
    }
    return count;
}

char **args_from_token_alloc(t_token *start, t_token *end, int count)
{
    char **args = malloc(sizeof(char *) * (count + 1));
    if (!args) return NULL;
    
    // Fill the args array
    int i = 0;
    t_token *tmp = start;
    while (tmp && tmp != end && tmp->type != TOKEN_PIPE) {
        if (tmp->type == TOKEN_WORD && 
            !(tmp->prev && (tmp->prev->type == TOKEN_REDIRECT_IN || 
                           tmp->prev->type == TOKEN_REDIRECT_OUT || 
                           tmp->prev->type == TOKEN_REDIRECT_APPEND || 
                           tmp->prev->type == TOKEN_HEREDOC))) {
            args[i++] = ft_strdup(tmp->value);
        }
        tmp = tmp->next;
    }
    args[i] = NULL;
    
    return args;
}


// Helper function to create args array from tokens
char **create_args_from_tokens(t_token *start, t_token *end)
{
    // int count = 0;
    // t_token *tmp = start;
    
    // // Count the number of word tokens that aren't redirection targets
    // while (tmp && tmp != end && tmp->type != TOKEN_PIPE) {
    //     if (tmp->type == TOKEN_WORD && 
    //         !(tmp->prev && (tmp->prev->type == TOKEN_REDIRECT_IN || 
    //                        tmp->prev->type == TOKEN_REDIRECT_OUT || 
    //                        tmp->prev->type == TOKEN_REDIRECT_APPEND || 
    //                        tmp->prev->type == TOKEN_HEREDOC))) {
    //         count++;
    //     }
    //     tmp = tmp->next;
    // }
    if(!start)
        return NULL;
    int count = count_args_seg(start,end);
    return args_from_token_alloc(start,end,count);
    
    // Allocate the args array
    // char **args = malloc(sizeof(char *) * (count + 1));
    // if (!args) return NULL;
    
    // // Fill the args array
    // int i = 0;
    // tmp = start;
    // while (tmp && tmp != end && tmp->type != TOKEN_PIPE) {
    //     if (tmp->type == TOKEN_WORD && 
    //         !(tmp->prev && (tmp->prev->type == TOKEN_REDIRECT_IN || 
    //                        tmp->prev->type == TOKEN_REDIRECT_OUT || 
    //                        tmp->prev->type == TOKEN_REDIRECT_APPEND || 
    //                        tmp->prev->type == TOKEN_HEREDOC))) {
    //         args[i++] = ft_strdup(tmp->value);
    //     }
    //     tmp = tmp->next;
    // }
    // args[i] = NULL;
    
    // return args;
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
        // if (current->type == TOKEN_REDIRECT_OUT) {
        //     current = current->next;
        //     if (current && current->type == TOKEN_WORD) {
        //         if (*out_fd != STDOUT_FILENO)
        //             close(*out_fd);
        //         *out_fd = open(current->value, O_WRONLY | O_CREAT | O_TRUNC, 0644);
        //         if (*out_fd < 0) {
        //             perror("open failed");
        //             exit(EXIT_FAILURE);
        //         }
        //     }
        // } else if (current->type == TOKEN_REDIRECT_APPEND) {
        //     current = current->next;
        //     if (current && current->type == TOKEN_WORD) {
        //         if (*out_fd != STDOUT_FILENO)
        //             close(*out_fd);
        //         *out_fd = open(current->value, O_WRONLY | O_CREAT | O_APPEND, 0644);
        //         if (*out_fd < 0) {
        //             perror("open failed");
        //             exit(EXIT_FAILURE);
        //         }
        //     }
        // } else if (current->type == TOKEN_REDIRECT_IN) {
        //     current = current->next;
        //     if (current && current->type == TOKEN_WORD) {
        //         if (*in_fd != STDIN_FILENO)
        //             close(*in_fd);
        //         *in_fd = open(current->value, O_RDONLY);
        //         if (*in_fd < 0) {
        //             perror("open failed");
        //             exit(EXIT_FAILURE);
        //         }
        //     }
        // } else if (current->type == TOKEN_HEREDOC) {
        //     current = current->next;
        //     if (current && current->type == TOKEN_WORD) {
        //         // Process heredoc
        //         if (*in_fd != STDIN_FILENO)
        //             close(*in_fd);
                
        //         int pipe_fd[2];
        //         if (pipe(pipe_fd) == -1) {
        //             perror("pipe failed");
        //             exit(EXIT_FAILURE);
        //         }
                
        //         heredoc_input = handler_heredoc(current->value, env);
        //         write(pipe_fd[1], heredoc_input, ft_strlen(heredoc_input));
        //         close(pipe_fd[1]);
        //         free(heredoc_input);
                
        //         *in_fd = pipe_fd[0];
        //     }
        // }
        
        if (current)
            current = current->next;
    }
}
