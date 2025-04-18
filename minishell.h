/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bolcay <bolcay@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/10 14:20:41 by bolcay            #+#    #+#             */
/*   Updated: 2025/04/17 13:41:56 by bolcay           ###   ########.fr       */
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
typedef struct s_builtin
{
	const char	*name;
	int		(*foo)(char **args);
}	t_builtin;

typedef struct s_env
{
	char	**envp;
	char	**export;
	int		exit_code;
	char	*curr_pwd;
	char	*old_pwd;
	char	**path;
	char	*path1;
	int		syntax_error;
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
void	append_exp(char *str, t_env *env);
void	append_env(char *str, t_env *env);
int		duplicate_check_env(char *str, t_env *env);
int		duplicate_check_ex(char *args, t_env *env);
void	duplicate_fix_env(char *str, t_env *env);
void	duplicate_fix_ex(char *str, t_env *env);
void	run_builtin(char **args, t_env *env);
void	run_export(char **args, t_env *env);
void	run_exit(char **args, t_env *env);
void	run_echo(char **args, t_env *env);
void	run_pwd(char **args, t_env *env);
void	run_unset(char **args, t_env *env);
void	run_env(char **args, t_env *env);
void	run_cd(char **args, t_env *env);

// Env functions

void	initiate_env(t_env *env, char **envp);
int		key_size(char *str);
int		value_size(char *str);
int		env_size(char **envp);
char	*find_path(t_env *env);
void	copy_env(char **str, char ***envp);
char	**update_env(char **envp, char *key);
char	**remove_env(char **envp, char *key);

// Execute functions
char	*find_exec(char *command, char *path_variable, int i, int j);
void	handle_redirection(t_token **current, char **args, int *out_fd, char **heredoc_input, t_env *env);
void	execute_with_redirection(char **args, t_env *env, int out_fd, int save_stdout);
void apply_redirections(t_token *start, t_token *end, int *in_fd, int *out_fd, t_env *env);
void	exec_without_pipes(t_token *tokens, t_env *env);
char **create_args_from_tokens(t_token *start, t_token *end);
t_token **find_pipe_seg(t_token *tokens, int *seg_count);
void	cell_launch(t_token *tokens, t_env *env);
void exec_command(char **args, t_env *env, int out_fd);

// execute with token functions
void openfile_redirected(t_token **current, int *out_fd, char **args, int append);
void    process_child_heredoc(t_token **current, char **heredoc_input, char **args, t_env *env);
void	read_redirected_in(t_token **current, int *in_fd, char **args, t_env *env);


// execute with pipe functions
void execute_piped_command(t_token *tokens, t_env *env);
void find_seg_redirect(int *in_fd, int *out_fd, t_token *start, t_token *end, t_env *env);
void setup_child_pipes(int **pipes, int i, int seg_count, int *in_fd, int *out_fd);
void exec_child_comd(t_token *seg_start, t_token *seg_end, t_env *env, int **pipes, int i, int seg_count);
int fork_cmd_process(t_token **segments, int seg_count, t_env *env, int **pipes, pid_t *pids);

// Pipe_utils functions
int has_pipes(t_token *tokens);
int **create_pipes(int seg_count);
void wait_child_pipes(pid_t *pids, int seg_count, t_env *env);
void cleanup_pipes(int **pipes, int seg_count);
int setup_io(int in_fd, int out_fd);

// Parsing
void apply_redirections(t_token *start, t_token *end, int *in_fd, int *out_fd, t_env *env);
void handle_heredoc_redirec(t_token **curr, int *in_fd,t_env *env);
void handle_standard_redirec(t_token **curr, int *in_fd, int *out_fd);
char **create_args_from_tokens(t_token *start, t_token *end);

// Parsing utils
int count_pipe_seg(t_token * tokens);
t_token **seg_alloc(t_token *tokens, int seg_count);
t_token **find_pipe_seg(t_token *tokens, int *seg_count);
int count_args_seg(t_token *start,t_token *end);
char **args_from_token_alloc(t_token *start, t_token *end, int count);

// Heredoc functions
int	expanded_heredoc_env(char *line, int *i, char **result, t_env *env);
char *expanded_heredoc_line(char *line, t_env *env);
char	*process_heredoc_line(char *line, char *result, t_env *env);
char *handler_heredoc(char *delimiter, t_env *env);
char	*ft_strjoin_heredoc(char const *s1, char const *s2);

// Deallocation functions
void	clean_2d(char **str);

// Signal functions
void    signal_mode_read(void);
void    sigint_handler_read(int signo);
void    signal_mode_command(void);
void    sigint_handler_command(int signo);
void    turn_off_echo(void);

// Tokenizer
t_token    *tokenizer(char *line, t_env *env);
void    seperated_token(char *line, t_token **head, t_env *env);
char    *extract_word(char *line, int *i);
void    free_token_matrix(t_token *token);
char *process_quoted(char *line, int *i, char quote_type, t_env *env);
char *expand_env(char *line, int *i, t_env *env);
char **tokens_to_args(t_token *tokens);

// seperated tokenizer
char  *handle_tilde(char *line, int *i, t_env *env);
void    handle_pipe(t_token *token, int *i);
void    handle_redirect_in(t_token *token, char *line, int *i);
void    handle_redirect_out(t_token *token, char *line, int *i);
void    handle_semic(t_token *token, int *i);
void    handle_word(t_token *token, char *line, int *i, t_env *env);
t_token *handle_variable_assign(char *line, int *i);
int assign_with_quoted(char *line, int i);
// Tokenizer utils
int ft_strcmp(const char *s1, const char *s2);
// Validate syntax for redirect
void    print_syntax_message(char *str, t_env *env);
int validate_redirect_syntax(t_token *tokens, t_env *env);
int validate_pipe_syntax(t_token *tokens, t_env *env);
int validate_syntax(t_token *tokens, t_env *env);


#endif