/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bolcay <bolcay@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/10 14:20:41 by bolcay            #+#    #+#             */
/*   Updated: 2025/04/02 14:45:54 by bolcay           ###   ########.fr       */
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

#define RESET        "\001\033[0m\002"   // Reset to default
#define CYAN         "\001\033[1;36m\002"
#define BLUE  		 "\001\033[1;34m\002"
#define RED   		 "\001\033[1;31m\002"
#define MGNT   		 "\001\033[1;35m\002"
#define GRN   		 "\001\033[1;32m\002"

# define DEBUG 1
	#if DEBUG
		# define DEBUG_PRINT(fmt, ...) printf(" [DEBUG] " fmt, ##__VA_ARGS__)
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
}	t_env;

typedef struct s_pipe_command
{
	int count_pipe;
    int index;
    pid_t pipe_id;
    int pipe_stdin;
    int pipe_stdout;
    char **args;
} t_pipe_command;

typedef struct s_pipe
{
	int nbr_command;
	int index;
	pid_t pipe_id;
	int pipe_stin;
	int pipe_stout;
	char **args;
}	t_pipe;

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
	TOKEN_SPACE,
	TOKEN_COMMAND,
	TOKEN_ARGUMENTS,
	TOKEN_VAR,
	TOKEN_EXPAND,
	TOKEN_EXIT,
	TOKEN_FILE,
}	t_token_type;


typedef	struct s_token
{
	char *value;
	int type;
	struct s_token *next; // next token
	struct s_token *prev;
}	t_token;


typedef struct s_cmd
{
	char *command;
	char *exc_path;
	char **args;
	struct s_cmd *next;
	struct s_cmd *prev;
}	t_cmd;


typedef struct s_parse
{
	t_token **head;
	t_env *env;
	char *user;
	int *i;
	char *buffer;
	int *buf_i;
} t_parse;

// Builtin functions

int		key_check(char *args);
int		builtin_check(char **tokens);
void	run_builtin(char **args, t_env *env);
void	run_export(char **args, t_env *env);
void	run_exit(void);
void	run_echo(char **args, t_env *env);
char 	*echo_separate(char *line, int check);
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
void	cell_launch(t_token *tokens, t_env *env);
void exec_command(char **args, t_env *env, int out_fd);
// char **tokens_to_args(t_token *tokens, t_token *end);
// execute with token functions
void openfile_redirected(t_token **current, int *out_fd, char **args, int append);
void    process_child_heredoc(t_token **current, char **heredoc_input, char **args, t_env *env);
void	read_redirected_in(t_token **current, int *in_fd, char **args, t_env *env);
// execute with pipe functions
void execute_pipes(t_pipe_command *pipes, t_env *env);
t_pipe_command *parse_pipe(t_token *tokens, int *pipe_count);
char **split_args(char *command);
char **split_pipes(char *command);
// Pipe_utils functions
void *ft_realloc(void *ptr, size_t size);
size_t strspn(const char *s, const char *accept);
size_t strcspn(const char *s, const char *reject);
char *strtok(char *str, const char *delim);


// execute_pipe functions
t_pipe    *initialized_pipe(char *current, int *comd_count);
// Pipe utils
char   *ft_strtok(char *restrict str, const char *restrict sep);
size_t ft_strcspn(const char *str, const char *reject);
void *ft_realloc(void *ptr, size_t size);

// Heredoc functions
int	expanded_heredoc_env(char *line, int *i, char **result, t_env *env);
char *expanded_heredoc_line(char *line, t_env *env);
char	*process_heredoc_line(char *line, char *result, t_env *env);
char *handler_heredoc(char *delimiter, t_env *env);
char	*ft_strjoin_heredoc(char const *s1, char const *s2);

// Deallocation functions

void	clean_2d(char **str);

// Readline functions
void    read_line(void);

// Signal functions
void    signal_mode_read(void);
void    sigint_handler_read(int signo);
void    signal_mode_command(void);
void    sigint_handler_command(int signo);
void    turn_off_echo(void);
void    rest_signal_command(void);


// Tokenizer
t_token    *tokenizer(char *line, t_env *env);
void    seperated_token(char *line, t_token **head, t_env *env);
char    *extract_word(char *line, int *i);
void    free_token_matrix(t_token *head);
char *process_quoted(char *line, int *i, char quote_type, t_env *env);
char *expand_env(char *line, int *i, t_env *env);
char **tokens_to_args(t_token *tokens, t_token *end);
// seperated tokenizer
void    handle_pipe(t_token *token, int *i);
void    handle_redirect_in(t_token *token, char *line, int *i);
void    handle_redirect_out(t_token *token, char *line, int *i);
void    handle_semic(t_token *token, int *i);
void    handle_word(t_token *token, char *line, int *i, t_env *env);
// Tokenizer utils
int ft_strcmp(const char *s1, const char *s2);


#endif