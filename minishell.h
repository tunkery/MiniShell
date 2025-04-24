/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bolcay <bolcay@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/10 14:20:41 by bolcay            #+#    #+#             */
/*   Updated: 2025/04/24 18:38:29 by bolcay           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "../libft/libft.h"
# include "get_next_line/get_next_line.h"
# include <stdio.h>                // printf, perror
# include <stdlib.h>               // malloc, free, exit, getenv
# include <unistd.h>               // write, access, read, close, fork, getcwd, chdir, unlink, dup, dup2, pipe, isatty, ttyname, ttyslot
# include <readline/readline.h> 
# include <readline/history.h> 
# include <fcntl.h>                // open
# include <sys/types.h>            // wait, waitpid, wait3, wait4, stat, lstat, fstat
# include <sys/wait.h>             // wait, waitpid, wait3, wait4
# include <signal.h>               // signal, sigaction, sigemptyset, sigaddset, kill
# include <sys/stat.h>             // stat, lstat, fstat
# include <dirent.h>               // opendir, readdir, closedir
# include <string.h>               // strerror
# include <termios.h>              // tcsetattr, tcgetattr
# include <curses.h>               // tgetent, tgetflag, tgetnum, tgetstr, tgoto, tputs
# include <sys/ioctl.h>            // ioctl
# include <sys/termios.h>          // tcsetattr, tcgetattr
# include <sys/unistd.h> 
# include <limits.h>

#define RESET        "\001\033[0m\002"   // Reset to default
#define CYAN         "\001\033[1;36m\002"
#define BLUE  		 "\001\033[1;34m\002"
#define RED   		 "\001\033[1;31m\002"
#define MGNT   		 "\001\033[1;35m\002"
#define GRN   		 "\001\033[1;32m\002"

# define DEBUG 1
	#if DEBUG
		# define DEBUG_PRINT(fmt, ...) fprintf(stderr, " [DEBUG] " fmt, ##__VA_ARGS__)
	#else
		# define DEBUG_PRINT(fmt, ...) do {} while(0) // Dont do anything when the debug closed
#endif

typedef struct s_gc_node
{
	void				*ptr;
	struct s_gc_node	*next;
}	t_gc_node;

typedef struct s_gc
{
	t_gc_node	*head;
}	t_gc;


typedef struct s_env
{
	t_gc	*gc;
	t_gc	*s_gc;
	char	**envp;
	char	**export;
	int		exit_code;
	char	*curr_pwd;
	char	*old_pwd;
	char	**path;
	char	*path1;
	int		syntax_error;
	int 	save_stdin;
}	t_env;

typedef struct s_pipe_command
{
	int count_pipe;
    int index;
    pid_t pipe_id;
    int pipe_stdin;
    int pipe_stdout;
    char **args;
	// t_token *start_token;
	// t_token *end_token;
} t_pipe_command;


typedef struct s_process
{
	char	*line;
	char	**envp;
}	t_process;


typedef enum e_token_type
{
	TOKEN_WORD, // (Cmd, Args)
	TOKEN_PIPE, // |
	TOKEN_REDIRECT_IN, // <
	TOKEN_REDIRECT_OUT, // >
	TOKEN_REDIRECT_APPEND, // >>
	TOKEN_HEREDOC, // <<
	TOKEN_HEREDOC_PROCESSED,
	TOKEN_SEMIC, // ;
	TOKEN_END,
}	t_token_type;


typedef	struct s_token
{
	t_token_type  type; // token type
	char  *value; // token value
	struct s_token *next;
	struct s_token *prev; // next token
}	t_token;

// Garbage collector

t_gc    *gc_new(void);
void    *my_malloc(t_gc *gc, int size);
void    gc_register(t_gc *gc, void *ptr);
void    gc_free_all(t_gc *gc);
void    gc_unregister(t_gc *gc, void *ptr);

// Builtin functions

int		key_check(char *args);
int		builtin_check(char **tokens);
void	checker(char *str, int name_c, int *check, t_env *env);
void	export1(char *str, t_env *env);
int		append_check(char *str);
int		append_key_size(char *str);
char	*append_organiser(char *str);
void	append(char *str, t_env *env, char *value, int i);
void	append1(char *str, t_env *env, char *value, int i);
char	*append2(char *str, char *value);
void	append_exp(char *str, t_env *env, int i);
void	append_env(char *str, t_env *env, int i);
int		duplicate_check_env(char *str, t_env *env);
int		duplicate_check_ex(char *args, t_env *env);
void	duplicate_fix_env(char *str, t_env *env);
void	duplicate_fix_ex(char *str, t_env *env, int i, int size);
void	run_builtin(char **args, t_env *env);
void	run_export(char **args, t_env *env);
void	run_exit(char **args, t_env *env);
void	run_echo(char **args, t_env *env);
void	run_pwd(char **args, t_env *env);
void	run_unset(char **args, t_env *env);
void	run_env(char **args, t_env *env);
void	run_cd(char **args, t_env *env);
void	cd_helper(char *args, t_env *env, char *old_pwd);
void	cd_tilde(char *path, t_env *env);
void	cd_minus_sign(char *args, char *path, t_env *env, char *old_pwd);
int		argument_check(char **args, t_env *env);
int		no_path_handle(t_env *env);
char	*get_old_pwd(t_env *env);
void	uop_helper(t_env *env, int j);
void	update_old_pwd(t_env *env, int check);
void	update_pwd(t_env *env);
int		valid_name_unset(char *str);
int		hey(char *str, char c);
char	*extract_c(char *str, char c);
int		unset_checker(char *args, t_env *env);
int		unset_checker2(char *args, t_env *env);

// Env functions

char	*ft_substr_ex(char const *s, unsigned int start, size_t len);
void	initiate_env(t_env *env, char **envp);
int		key_size(char *str);
int		value_size(char *str);
int		env_size(char **envp);
char	*find_path(t_env *env);
void	copy_env(char **str, char ***envp, t_env *env);
void	copy_ex(char **str, char ***envp, t_env *env);
char	*copy_ex_helper(char *str);
char	**update_ex(char **envp, char *key, t_env *env);
char	**update_env(char **envp, char *key, t_env *env);
char	**remove_env(char **envp, char *key, t_env *env);

// Execute functions
char	*find_exec(char *command, char *path_variable, int i, t_env *env);
void	handle_redirection(t_token **current, char **args, int *out_fd,
char	**heredoc_input, t_env *env);
void	execute_with_redirection(char **args, t_env *env, int out_fd, int save_stdout);
void	exec_without_pipes(t_token *tokens, t_env *env, int out_fd);
char	**create_args_from_tokens(t_token *start, t_token *end, t_env *env);
t_token **find_pipe_seg(t_token *tokens, int *seg_count, t_env *env);
void	cell_launch(t_token *tokens, t_env *env);
void	exec_command(char **args, t_env *env, int out_fd);

// execute with token functions
void 	openfile_redirected(t_token **current, int *out_fd, char **args, int append);
void    process_child_heredoc(t_token **current, char **heredoc_input, char **args, t_env *env);
void	read_redirected_in(t_token **current, int *in_fd, char **args, t_env *env);


// execute with pipe functions
void 	execute_piped_command(t_token *tokens, t_env *env);
void 	find_seg_redirect(int *in_fd, int *out_fd, t_token *start, t_token *end, t_env *env);
void 	setup_child_pipes(int **pipes, int i, int seg_count, int *in_fd, int *out_fd);
void 	exec_child_comd(t_token *seg_start, t_token *seg_end, t_env *env, int **pipes, int i, int seg_count);
int 	fork_cmd_process(t_token **segments, int seg_count, t_env *env, int **pipes, pid_t *pids);

// Pipe_utils functions

int		has_pipes(t_token *tokens);
int		**create_pipes(int seg_count);
void	wait_child_pipes(pid_t *pids, int seg_count, t_env *env);
void	cleanup_pipes(int **pipes, int seg_count);
int		setup_io(int in_fd, int out_fd);

// Parsing

void	apply_redirections(t_token *start, t_token *end, int *in_fd, int *out_fd, t_env *env);
void	handle_heredoc_redirec(t_token **curr, int *in_fd,t_env *env);
void	handle_standard_redirec(t_token **curr, int *in_fd, int *out_fd);
char	**create_args_from_tokens(t_token *start, t_token *end, t_env *env);

// Parsing utils

int		count_pipe_seg(t_token * tokens);
t_token **seg_alloc(t_token *tokens, int seg_count, t_env *env);
t_token **find_pipe_seg(t_token *tokens, int *seg_count, t_env *env);
int		count_args_seg(t_token *start,t_token *end);
char	**args_from_token_alloc(t_token *start, t_token *end, int count, t_env *env);
int		preprocess_heredocs(t_token **seg, int seg_count, t_env *env);

// Heredoc functions

int		expanded_heredoc_env(char *line, int *i, char **result, t_env *env);
char	*expanded_heredoc_line(char *line, t_env *env);
char	*process_heredoc_line(char *line, char *result, t_env *env);
char	*handler_heredoc(char *delimiter, t_env *env);
char	*ft_strjoin_heredoc(char const *s1, char const *s2);

// Deallocation functions

void	clean_2d(char **str);

// Signal functions

void    signal_mode_read(void);
void    sigint_handler_read(int signo);
void    signal_mode_command(void);
void    sigint_handler_command(int signo);
void    turn_off_echo(void);
void	set_signal_heredoc(void);
void	set_signal_pipe(void);

// Tokenizer

t_token	*tokenizer(char *line, t_env *env);
void	seperated_token(char *line, t_token **head, t_env *env);
char    *extract_word(char *line, int *i, t_env *env);
void    free_token_matrix(t_token *token);
char	*process_quoted(char *line, int *i, char quote_type, t_env *env);
void	process_quoted_helper(char **result, char *line, int *i, t_env *env);
void	process_quoted_helper2(char **result, char *line, int *i, t_env *env);
char	*expand_env(char *line, int *i, t_env *env);
char	**tokens_to_args(t_token *tokens, t_env *env);

// seperated tokenizer

char	*handle_tilde(char *line, int *i, t_env *env);
void    handle_pipe(t_token *token, int *i, t_env *env);
void    handle_redirect_in(t_token *token, char *line, int *i, t_env *env);
void    handle_redirect_out(t_token *token, char *line, int *i, t_env *env);
void    handle_semic(t_token *token, int *i, t_env *env);
void    handle_word(t_token *token, char *line, int *i, t_env *env);
t_token *handle_variable_assign(char *line, int *i);
int 	assign_with_quoted(char *line, int i);

// Tokenizer utils

int		ft_strcmp(const char *s1, const char *s2);
char	*join_and_register(char *result, char *temp, t_env *env);
char	*handle_word_helper(char *result, int *i, t_env *env);
int		handle_word_helper2(char **result, char *line, int *i, t_env *env);
int		token_alloc_check(t_token *tmp);
int		token_alloc_check1(t_token *tmp);
void	pid_is_zero(int *pipe, char **heredoc_input, t_env *env, t_token *curr);
int		pipe_helper(int *pip, char **heredoc_input, t_env *env, t_token *curr);
int	parent_process_heredoc_pipe(int *pipe_fd, pid_t pid, t_token *curr,
	t_env *env);
void	heredoc_helper(char **heredoc, t_token **curr, int *pipe_fd, t_env *env);
int	token_to_args_helper(t_token **temp);
void	token_to_args_helper1(char ***args, t_token *tmp, t_env *env, int *i);
int		count_token_helper(t_token *tokens);
void	close_both(int save_stdout, int save_stdin);
void	write_it(t_token **current);
void	run_with_path_helper(char *str, char **args, t_env *env, int out_fd);
void	wait_for_child(pid_t pid, t_env *env);
// Validate syntax for redirect

int		print_syntax_message(char *str, t_env *env);
int		validate_redirect_syntax(t_token *tokens, t_env *env);
int		validate_pipe_syntax(t_token *tokens, t_env *env, int check);
int		validate_syntax(t_token *tokens, t_env *env);


#endif