/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exe_main.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bolcay <bolcay@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/24 12:04:28 by hpehliva          #+#    #+#             */
/*   Updated: 2025/03/25 13:25:58 by bolcay           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static int count_token_args(t_token *tmp)
{
	int count;

	count = 0;
	while (tmp &&  tmp->type != TOKEN_SEMIC && tmp->type != TOKEN_REDIRECT_APPEND
		&& tmp->type != TOKEN_HEREDOC && tmp->type != TOKEN_REDIRECT_OUT
		&& tmp->type != TOKEN_REDIRECT_IN && tmp->type != TOKEN_HEREDOC
		&& tmp->type != TOKEN_PIPE)
	{
		if (tmp->type == TOKEN_WORD)
			count++;
		tmp = tmp->next;
	}
	return (count);
}

char	**tokens_to_args(t_token *tokens)
{
	// int		count;
	t_token	*tmp;
	char	**args;
	int		i;

	// count = 0;
	tmp = tokens;
	i = 0;
	args = malloc((count_token_args(tokens) + 1) * sizeof(char *));
	if (!args)
		return (NULL);
	tmp = tokens;
	while (tmp && tmp->type != TOKEN_SEMIC && tmp->type != TOKEN_REDIRECT_APPEND
		&& tmp->type != TOKEN_HEREDOC && tmp->type != TOKEN_REDIRECT_OUT
		&& tmp->type != TOKEN_REDIRECT_IN && tmp->type != TOKEN_HEREDOC
		&& tmp->type != TOKEN_PIPE)
	{
		if (tmp->type == TOKEN_WORD)
			args[i++] = ft_strdup(tmp->value);
				// change strdup to ft_strdup and test it again //TODO
		tmp = tmp->next;
	}
	args[i] = NULL;
	return (args);
}

void	execute_with_redirection(char **args, t_env *env, int out_fd,
		int save_stdout)
{
	if (builtin_check(args) != 0)
	{
		if (out_fd != STDOUT_FILENO)
		{
			dup2(out_fd, STDOUT_FILENO);
			if (dup2(out_fd, STDOUT_FILENO) == -1)
				perror("dup2 failed!");
			close(out_fd);
			DEBUG_PRINT(GRN "Output redirected to file\n" RESET);
		}
		run_builtin(args, env);
		if (out_fd != STDOUT_FILENO)
		{
			dup2(save_stdout, STDOUT_FILENO);
			if (dup2(save_stdout, STDOUT_FILENO) == -1)
				perror("dup2 failed!");
			DEBUG_PRINT(GRN "STDOUT restored\n" RESET);
		}
	}
	else
	{
		exec_command(args, env, out_fd);
		DEBUG_PRINT(GRN "Command executed\n" RESET);
	}
}

void	handle_redirection(t_token **current, char **args, int *out_fd,
		char **heredoc_input, t_env *env)
{
	int in_fd = STDIN_FILENO;
	while (*current && (*current)->type != TOKEN_SEMIC && (*current)->type != TOKEN_PIPE)
	{
		/* Sembolu eklemek */
		if ((*current)->type == TOKEN_REDIRECT_OUT) // >
		{
			*current = (*current)->next;
			openfile_redirected(current, out_fd, args, 0);
		}
		else if (*current && (*current)->type == TOKEN_REDIRECT_IN) // <
		{
			*current = (*current)->next;
			read_redirected_in(current, &in_fd, args, env);
			if(!*args)
				return ;
		}
		else if ((*current)->type == TOKEN_REDIRECT_APPEND) // >>
		{
			*current = (*current)->next;
			openfile_redirected(current, out_fd, args, 1);
		}
		else if ((*current)->type == TOKEN_HEREDOC) // <<
		{
			*current = (*current)->next;
			process_child_heredoc(current, heredoc_input, args, env);
		}
		else
			(*current) = (*current)->next;
	}
}
t_token **find_pipe_seg(t_token *tokens, int *seg_count)
{
    t_token *tmp = tokens;
    int count = 0;
    
    // Count segments (commands separated by pipes)
    while (tmp && tmp->type != TOKEN_SEMIC) {
        if (tmp->type == TOKEN_PIPE)
            count++;
        tmp = tmp->next;
    }
    count++; // Add one for the last segment
    
    // Allocate array for segment pointers
    t_token **segments = malloc(sizeof(t_token *) * (count + 1));
    if (!segments)
        return NULL;
    
    // Find start of each segment
    tmp = tokens;
    int i = 0;
    segments[i++] = tmp; // First segment starts at the beginning
    
    while (tmp && tmp->type != TOKEN_SEMIC) {
        if (tmp->type == TOKEN_PIPE) {
            segments[i++] = tmp->next; // Next segment starts after pipe
        }
        tmp = tmp->next;
    }
    
    segments[i] = NULL; // End marker
    *seg_count = count;
    
    return segments;

}

void	exec_command_seg(t_token *start, t_token *end, t_env *env)
{
    char **args = NULL;
    int out_fd = STDOUT_FILENO;
    int save_stdout = dup(STDOUT_FILENO);
    int save_stdin = dup(STDIN_FILENO);
    char *heredoc_input = NULL;
    t_token *current = start;
    
    // Extract arguments (excluding redirection tokens and their targets)
    int arg_count = 0;
    t_token *tmp = start;
    
    while (tmp && tmp != end && tmp->type != TOKEN_PIPE) {
        if (tmp->type == TOKEN_WORD && 
            !(tmp->prev && (tmp->prev->type == TOKEN_REDIRECT_OUT || 
                           tmp->prev->type == TOKEN_REDIRECT_APPEND || 
                           tmp->prev->type == TOKEN_REDIRECT_IN || 
                           tmp->prev->type == TOKEN_HEREDOC))) {
            arg_count++;
        }
        tmp = tmp->next;
    }
    
    args = malloc(sizeof(char *) * (arg_count + 1));
    if (!args) {
        perror("malloc failed");
        return;
    }
    
    int i = 0;
    tmp = start;
    
    while (tmp && tmp != end && tmp->type != TOKEN_PIPE) {
        if (tmp->type == TOKEN_WORD && 
            !(tmp->prev && (tmp->prev->type == TOKEN_REDIRECT_OUT || 
                           tmp->prev->type == TOKEN_REDIRECT_APPEND || 
                           tmp->prev->type == TOKEN_REDIRECT_IN || 
                           tmp->prev->type == TOKEN_HEREDOC))) {
            args[i++] = ft_strdup(tmp->value);
        }
        tmp = tmp->next;
    }
    args[i] = NULL;
    
    // Handle redirections for this segment
    handle_redirection(&current, args, &out_fd, &heredoc_input, env);
    
    // Execute the command
    if (args && args[0]) {
        execute_with_redirection(args, env, out_fd, save_stdout);
    }
    
    // Clean up
    if (args)
        clean_2d(args);
    if (out_fd != STDOUT_FILENO)
        close(out_fd);
    close(save_stdout);
    close(save_stdin);

}


// a function that runs the programs in the computer
/*
	NOTES:
	- The dup() system call creates a duplicate of the file descriptor oldfd,
		using the lowest-numbered unused file descriptor for the new descriptor.
	- The dup2() system call performs the same task as dup(),
		but instead of using the lowest-numbered unused file descriptor,
		it uses the file descriptor number specified in newfd.

*/

void	cell_launch(t_token *tokens, t_env *env)
{
	t_token	*tmp;
	char	**args;
	int		out_fd;
	int		save_stdout;
	int		save_stdin;
	char	*heredoc_input;
	// int		pipe_count = 0;
	// t_pipe_command *pipes;
	int		i;

	tmp = tokens;
	args = NULL;
	out_fd = STDOUT_FILENO;
	save_stdout = dup(STDOUT_FILENO);
	save_stdin = dup(STDIN_FILENO);
	heredoc_input = NULL;
	DEBUG_PRINT(BLUE "Starting Cell_lounch\n" RESET);

	// if(has_pipes(tokens))
	// {
	// 	DEBUG_PRINT(BLUE "Pipe detected, using pipe execution path\n" RESET);
	// 	pipes = parse_pipe(tokens, &pipe_count);
	// 	if (pipes)
	// 	{
	// 		execute_pipes(pipes, env, tokens);
	// 		free_pipe_command(pipes, pipe_count);
	// 		// i = 0;
	// 		// while(i < pipe_count)
	// 		// {
	// 		// 	if(pipes[i].args)
	// 		// 		clean_2d(pipes[i].args);
	// 		// 	i++;
	// 		// }
	// 		// free(pipes);
	// 		return ;
	// 	}
	// 	DEBUG_PRINT(RED "Pipe parsing failed, falling back to normal execution\n" RESET);
	// }
	// if(has_pipes(tokens))
	// {
	// 	int segment_count = 0;
	// 	t_token **segments = find_pipe_seg(tokens, &segment_count);
		
	// 	if (segments) {
	// 		// Create pipes between segments
	// 		// Fork processes for each segment
	// 		for (int i = 0; i < segment_count; i++) {
	// 			// Determine segment end (next segment start or NULL)
	// 			t_token *segment_end = (i < segment_count - 1) ? segments[i+1] : NULL;
	// 			exec_command_seg(segments[i], segment_end, env);
	// 		}
	// 		free(segments);
	// 		return;
	// 	}
	// }
	if(has_pipes(tokens))
{
    DEBUG_PRINT(BLUE "Pipe detected, using segment-based execution\n" RESET);
    int segment_count = 0;
    t_token **segments = find_pipe_seg(tokens, &segment_count);
    
    if (segments) {
        // Create pipes
        int pipe_fds[segment_count - 1][2];
        for (int i = 0; i < segment_count - 1; i++) {
            if (pipe(pipe_fds[i]) == -1) {
                perror("pipe failed");
                free(segments);
                return;
            }
        }
        
        // Fork processes for each segment
        pid_t pids[segment_count];
        for (int i = 0; i < segment_count; i++) {
            pids[i] = fork();
            
            if (pids[i] < 0) {
                perror("fork failed");
                // Clean up
                for (int j = 0; j < segment_count - 1; j++) {
                    close(pipe_fds[j][0]);
                    close(pipe_fds[j][1]);
                }
                free(segments);
                return;
            }
            
            if (pids[i] == 0) { // Child process
                // Set up stdin for all but the first command
                if (i > 0) {
                    if (dup2(pipe_fds[i-1][0], STDIN_FILENO) == -1) {
                        perror("dup2 for stdin failed");
                        exit(EXIT_FAILURE);
                    }
                }
                
                // Set up stdout for all but the last command
                if (i < segment_count - 1) {
                    if (dup2(pipe_fds[i][1], STDOUT_FILENO) == -1) {
                        perror("dup2 for stdout failed");
                        exit(EXIT_FAILURE);
                    }
                }
                
                // Close all pipe fds in the child
                for (int j = 0; j < segment_count - 1; j++) {
                    close(pipe_fds[j][0]);
                    close(pipe_fds[j][1]);
                }
                
                // Determine segment end
                t_token *segment_end = (i < segment_count - 1) ? segments[i+1] : NULL;
                
                // Execute the command segment
                exec_command_seg(segments[i], segment_end, env);
                exit(env->exit_code);
            }
        }
        
        // Parent process: close all pipe fds
        for (int i = 0; i < segment_count - 1; i++) {
            close(pipe_fds[i][0]);
            close(pipe_fds[i][1]);
        }
        
        // Wait for all child processes
        for (int i = 0; i < segment_count; i++) {
            int status;
            waitpid(pids[i], &status, 0);
            
            if (WIFEXITED(status)) {
                env->exit_code = WEXITSTATUS(status);
            } else if (WIFSIGNALED(status)) {
                env->exit_code = 128 + WTERMSIG(status);
            }
        }
        
        free(segments);
        return;
    }
}

	while (tmp)
	{
		args = tokens_to_args(tmp);
		if (!args)
		{
			DEBUG_PRINT(RED "Failed to convert tokens to args\n" RESET);
			while (tmp && tmp->type != TOKEN_SEMIC)
				tmp = tmp->next;
			if (tmp)
				tmp = tmp->next;
			continue ;
		}
		DEBUG_PRINT(BLUE "Args created\n" RESET);
		/* Check it all aguments in here*/
		i = 0;
		while (args[i])
		{
			DEBUG_PRINT(BLUE "Args[%d]: %s\n" RESET, i, args[i]);
			i++;
		}
		handle_redirection(&tmp, args, &out_fd, &heredoc_input, env);
		if(args && args[0])
		{
			execute_with_redirection(args, env, out_fd, save_stdout);
			
		}
		DEBUG_PRINT(RED"Skip execution due to redirection failed!"RESET);
		// clean_2d(args);
		args = NULL;
		if (out_fd != STDOUT_FILENO)
			close(out_fd);
		out_fd = STDOUT_FILENO;
		if (dup2(save_stdin, STDIN_FILENO) == -1)
			perror("dup2 failed to restore STDIN");
		while (tmp && tmp->type != TOKEN_SEMIC)
		{
			DEBUG_PRINT(RED"Advancing tmp: %p, type: %d\n"RESET, tmp, tmp->type);
			tmp = tmp->next;
		}
		if (tmp)
		{
			DEBUG_PRINT(RED"Skipping semicol"RESET);
			tmp = tmp->next;
		}
	}
	close(save_stdout);
	close(save_stdin);
	DEBUG_PRINT(BLUE "Ending Cell_lounch\n" RESET);

}
