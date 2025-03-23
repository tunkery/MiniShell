/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bolcay <bolcay@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/10 14:20:41 by bolcay            #+#    #+#             */
/*   Updated: 2025/03/23 09:21:25 by bolcay           ###   ########.fr       */
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

#define RESET        "\001\033[0m"   // Reset to default
#define CYAN         "\001\033[1;36m"
#define BLUE  		 "\001\033[1;34m"
#define RED   		 "\001\033[1;31m"
#define GRN   		 "\001\033[1;32m"

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
	char	**path;
	char	*path1;
}	t_env;

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
	TOKEN_REDIRECT_APPEND,
	TOKEN_HEREDOC,
	TOKEN_SEMIC,
	TOKEN_END
}	t_token_type;


typedef	struct s_token
{
	t_token_type  type; // token type
	char  *value; // token value
	struct s_token *next; // next token
}	t_token;


// Builtin functions

int		key_check(char *args);
int		builtin_check(char **tokens);
void	run_builtin(char **args, t_env *env);
void	run_export(char **args, t_env *env);
void	run_exit(void);
void	run_echo(char **args);
void	run_pwd(char **args);
void	run_unset(char **args, t_env *env);
void	run_env(char **args, t_env *env);
void	run_cd(char **args);

// Env functions

char	*find_exec(char *command, char *path_variable, int i, int j);
void	cell_launch(t_token *tokens, t_env *env);
void	initiate_env(t_env *env, char **envp);
int		key_size(char *str);
int		value_size(char *str);
int		env_size(char **envp);
void	copy_env(char **str, char ***envp);
char	**update_env(char **envp, char *key);
char	**get_tokens(char *str);
char	**remove_env(char **envp, char *key);

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

// Process
void added_process(char *line, char **envp);
void exec_command(char **args, t_env *env, int out_fd);

// Tokenizer
t_token    *tokenizer(char *line);
void    seperated_token(char *line, t_token **head);
char    *extract_word(char *line, int *i);
void    free_token_matrix(t_token *head);
char *process_quoted(char *line, int *i, char quote_type);
char *expand_env(char *line, int *i);
char *handler_heredoc(char *delimiter);
char **tokens_to_args(t_token *tokens);

// Tokenizer utils
int ft_strcmp(const char *s1, const char *s2);


#endif