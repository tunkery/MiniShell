/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: batuhan <batuhan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/10 14:20:41 by bolcay            #+#    #+#             */
/*   Updated: 2025/04/22 19:44:38 by batuhan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "../libft/libft.h"
# include "get_next_line/get_next_line.h"
# include <curses.h> // tgetent, tgetflag, tgetnum, tgetstr, tgoto, tputs
# include <dirent.h> // opendir, readdir, closedir
# include <fcntl.h>  // open
# include <limits.h>
# include <readline/history.h>
# include <readline/readline.h>
# include <signal.h>      // signal, sigaction, sigemptyset, sigaddset, kill
# include <stdio.h>       // printf, perror
# include <stdlib.h>      // malloc, free, exit, getenv
# include <string.h>      // strerror
# include <sys/ioctl.h>   // ioctl
# include <sys/stat.h>    // stat, lstat, fstat
# include <sys/termios.h> // tcsetattr, tcgetattr
# include <sys/types.h>   // wait, waitpid, wait3, wait4, stat, lstat, fstat
# include <sys/unistd.h>
# include <sys/wait.h> // wait, waitpid, wait3, wait4
# include <termios.h>  // tcsetattr, tcgetattr
# include <unistd.h>   // write, access, read, close, fork, getcwd, chdir,

# define RESET "\001\033[0m\002" // Reset to default
# define CYAN "\001\033[1;36m\002"
# define BLUE "\001\033[1;34m\002"
# define RED "\001\033[1;31m\002"
# define MGNT "\001\033[1;35m\002"
# define GRN "\001\033[1;32m\002"

// # define DEBUG 1
// # if DEBUG
// #  define DEBUG_PRINT(fmt, ...) fprintf(stderr, " [DEBUG] " fmt,
//	##__VA_ARGS__);
// # else
// #  define DEBUG_PRINT(fmt, ...) \
// 	do                        \
// 	{                         \
// 	} while (0)
// # endif

typedef struct s_builtin
{
	const char			*name;
	int					(*foo)(char **args);
}						t_builtin;

typedef struct s_gc_node
{
	void				*ptr;
	struct s_gc_node	*next;
}						t_gc_node;

typedef struct s_gc
{
	t_gc_node			*head;
}						t_gc;

typedef struct s_env
{
	t_gc				*gc;
	t_gc				*s_gc;
	char				**envp;
	char				**export;
	int					exit_code;
	char				*curr_pwd;
	char				*old_pwd;
	char				**path;
	char				*path1;
	int					syntax_error;
	int					save_stdin;
	pid_t				*pids;
}						t_env;

typedef struct s_pipe_command
{
	int					count_pipe;
	int					index;
	pid_t				pipe_id;
	int					pipe_stdin;
	int					pipe_stdout;
	char				**args;
}						t_pipe_command;

typedef struct s_process
{
	char				*line;
	char				**envp;
}						t_process;

typedef enum e_token_type
{
	TOKEN_WORD,            // (Cmd, Args)
	TOKEN_PIPE,            // |
	TOKEN_REDIRECT_IN,     // <
	TOKEN_REDIRECT_OUT,    // >
	TOKEN_REDIRECT_APPEND, // >>
	TOKEN_HEREDOC,         // <<
	TOKEN_HEREDOC_PROCESSED,
	TOKEN_HEREDOC_DELIMITER,
	TOKEN_SEMIC, // ;
	TOKEN_END,
}						t_token_type;

typedef struct s_token
{
	t_token_type type; // token type
	char *value;       // token value
	struct s_token		*next;
	struct s_token *prev; // next token
	int quote_mode;       // 0: no quote, 1: single quote, 2: double quote
}						t_token;

typedef struct s_token_state
{
	char				*line;
	int					i;
	t_token				*current;
	t_token				**head;
	t_env				*env;
}						t_token_state;

typedef struct s_pipe_seg
{
	int					**pipes;
	int					seg_count;
	int					seg_index;
	int					fds[2];
	t_token				*seg_start;
	t_token				*seg_end;
}						t_pipe_seg;

// builtins_utils.c:
int						key_check(char *args);
int						builtin_check(char **tokens);
void					run_builtin(char **args, t_env *env);
// builtins_utils2.c
int						append_check(char *str);
char					*append_organiser(char *str);
int						append_key_size(char *str);
void					append_exp(char *str, t_env *env, int i);
char					*append2(char *str, char *value);
// builtin_utils3.c
int						duplicate_check_env(char *str, t_env *env);
int						duplicate_check_ex(char *str, t_env *env);
void					duplicate_fix_env(char *str, t_env *env);
void					duplicate_fix_ex(char *str, t_env *env, int i);
void					export1(char *str, t_env *env);
// builtins_utils4.c
void					checker(char *str, int name_c, int *check, t_env *env);
void					append_env(char *str, t_env *env, int i);
void					append1(char *str, t_env *env, char *value, int i);
void					append(char *str, t_env *env, char *value, int i);
// builtin_utils5.c
int						unset_checker(char *args, t_env *env);
int						unset_checker2(char *args, t_env *env);
int						exit_helper(const char *str, int i,
							unsigned long long *check);
void					exit_helper1(int *sign, const char *str, int *i,
							unsigned long long *check);
int						exit_helper2(char **args, int *i);
// cd_helper..c
int						no_path_handle(t_env *env);
int						argument_check(char **args, t_env *env);
void					cd_minus_sign(char *args, char *path, t_env *env,
							char *old_pwd);
void					cd_tilde(char *path, t_env *env);
void					cd_helper(char *args, t_env *env, char *old_pwd);
// cd.c
void					uop_helper(t_env *env, int j);
char					*get_old_pwd(t_env *env);
void					update_old_pwd(t_env *env, int check);
void					update_pwd(t_env *env);
void					run_cd(char **args, t_env *env);
// echo.c
void					run_echo(char **args, t_env *env);
// env.c
void					run_env(char **args, t_env *env);
// exit.c
void					run_exit(char **args, t_env *env);
// export.c
void					run_export(char **args, t_env *env);
// pwd.c
void					run_pwd(char **args, t_env *env);
// unset.c
int						valid_name_unset(char *str);
int						hey(char *str, char c);
char					*extract_c(char *str, char c);
void					run_unset(char **args, t_env *env);

/*ENVP*/
// envp_utils.c
int						key_size(char *str);
int						value_size(char *str);
int						env_size(char **envp);
void					copy_env(char **str, char ***envp, t_env *env);
char					*copy_ex_helper(char *str);
void					copy_ex(char **str, char ***envp, t_env *env);
// envp_utils2.c
char					*find_path(t_env *env);
// envp.c
char					**update_env(char **envp, char *key, t_env *env);
char					**update_ex(char **envp, char *key, t_env *env);
char					**remove_env(char **envp, char *key, t_env *env);
void					initiate_env(t_env *env, char **envp);
char					*find_exec(char *command, char *path, int i,
							t_env *env);

/*EXECUTE*/
char					**tokens_to_args(t_token *tokens, t_env *env);
void					execute_with_redirection(char **args, t_env *env,
							int out_fd, int save_stdout);
void					handle_redirection(t_token **current, char **args,
							int *out_fd, char **heredoc_input, t_env *env);
void					exec_without_pipes(t_token *tokens, t_env *env,
							int out_fd);
void					cell_launch(t_token *tokens, t_env *env);
// exe_utils.c
int						token_to_args_helper(t_token **temp);
void					token_to_args_helper1(char ***args, t_token *tmp,
							t_env *env, int *i);
int						count_token_helper(t_token *tokens);
void					close_both(int save_stdout, int save_stdin);
void					write_it(t_token **current);
// exe_utils1.c
void					run_with_path_helper(char *str, char **args, t_env *env,
							int out_fd);
void					create_pipe_helper(int ***pipes, int i);
void					create_pipe_helper1(int ***pipes, int i);
void					exe_helper(t_token **temp);
void					exe_helper1(int save_stdin);

// exe_with_token.c
void					read_redirected_in(t_token **current, int *in_fd,
							char **args, t_env *env);
void					process_child_heredoc(t_token **current,
							char **heredoc_input, char **args, t_env *env);
void					openfile_redirected(t_token **current, int *out_fd,
							char **args, int append);
// heredoc_utils.c
char					*ft_strjoin_heredoc(char const *s1, char const *s2);
int						expanded_heredoc_env(char *line, int *i, char **result,
							t_env *env);
char					*process_heredoc_line(char *line, char *result,
							t_env *env);
// heredoc.c
char					*ft_strjoin_free(char *s1, char *s2);
char					*expand_command_substitution(char *line, int *index,
							t_env *env);
char					*expanded_heredoc_line(char *line, t_env *env);
char					*handler_heredoc(char *delimiter, t_env *env,
							int quote_mode);
// pipe_utils.c
int						has_pipes(t_token *tokens);
int						**create_pipes(int seg_count);
void					wait_child_pipes(pid_t *pids, int seg_count,
							t_env *env);
void					cleanup_pipes(int **pipes, int seg_count);
int						setup_io(int in_fd, int out_fd);
// pipe.c
void					execute_piped_command(t_token *tokens, t_env *env);
int						setup_pipe_exec(t_token **segments, int seg_count,
							t_env *env, int ***pipes_ptr);
int						fork_cmd_process(t_token **segments, int seg_count,
							t_env *env, int **pipes);
int						create_single_process(t_token **segments, int i,
							t_pipe_seg *stat);
void					exec_child_comd(t_pipe_seg *stat, t_env *env);
void					exec_cmd_or_buildin(t_token *seg_start,
							t_token *seg_end, t_env *env);
void					setup_child_pipes(t_pipe_seg *stat, int *fds);
void					find_seg_redirect(int *fds, t_token *start,
							t_token *end, t_env *env);
void					init_pipe_seg(t_pipe_seg *stat, int **pipes,
							int seg_count, int i);

// run_commands.c
void					wait_for_child(pid_t pid, t_env *env);
void					exec_command(char **args, t_env *env, int out_fd);

/*PARSING*/
int						parent_process_heredoc_pipe(int *pipe_fd, pid_t pid,
							t_token *curr, t_env *env);
int						set_heredoc_pipe(int pipe_fd[2]);
int						create_heredoc_child(int pipe_fd[2], pid_t *pid);
void					execute_child_heredoc(int pipe_fd[2], t_token *curr,
							t_env *env);
int						process_heredoc_token(t_token *curr, t_env *env);
int						process_segment(t_token *curr, t_token *seg_end,
							t_env *env);
t_token					*find_segment_end(t_token **seg, int i, int seg_count);
int						preprocess_heredocs(t_token **seg, int seg_count,
							t_env *env);
int						count_pipe_seg(t_token *tokens);
t_token					**seg_alloc(t_token *tokens, int seg_count, t_env *env);
t_token					**find_pipe_seg(t_token *tokens, int *seg_count,
							t_env *env);
int						count_args_seg(t_token *start, t_token *end);
int						token_alloc_check1(t_token *tmp);
int						token_alloc_check(t_token *tmp);
char					**args_from_token_alloc(t_token *start, t_token *end,
							int count, t_env *env);
void					process_apply_redirections(t_token **current,
							int *in_fd, int *out_fd, t_env *env);
// parsing.c
char					**create_args_from_tokens(t_token *start, t_token *end,
							t_env *env);
void					handle_standard_redirec(t_token **curr, int *in_fd,
							int *out_fd);
void					setup_pipe_heredoc(t_token *curr, int *in_fd,
							t_env *env);
void					handle_heredoc_redirec(t_token **curr, int *in_fd,
							t_env *env);
void					apply_redirections(t_token *start, t_token *end,
							int *fds, t_env *env);

/*SIGNALS*/
// set_signal.c
void					turn_off_echo(void);
void					sigint_handler_command(int signo);
void					signal_mode_command(void);
void					sigint_handler_read(int signo);
void					signal_mode_read(void);
// set_signal1.c
void					set_signal_heredoc(void);
void					set_signal_pipe(void);

/*TOKENS*/
// tokenizer
void					process_delimiter_check(t_token *delimiter, char *line,
							int *i, t_env *env);
t_token					*handle_heredoc_token(int *i, t_token *current,
							t_token **head, t_env *env);
t_token					*create_delimiter_token(char *line, int *i,
							t_token *current, t_env *env);
void					process_token_type(t_token_state *state);
void					seperated_token(char *line, t_token **head, t_env *env);
t_token					*tokenizer(char *line, t_env *env);
// tokenizer_utils2.c
char					*regular_char_decrease(char *result, char *line, int *i,
							t_env *env);
char					*handle_quote_var(char *line, int *i, char *result,
							t_env *env);
// tokenizer_utils1.c
char					*ansi_c_quote(char *str, t_env *env);
char					*process_quoted(char *line, int *i, char quote_type,
							t_env *env);
t_token					*handle_special_token(char *line, int *i, t_env *env);
void					skip_whitespace(char *line, int *i);
t_token					*add_token_list(t_token *token, t_token **head,
							t_token *current);
// tokenizer_utils.c
int						ft_strcmp(const char *s1, const char *s2);
char					*handle_regular_char(char *result, char *line, int *i,
							t_env *env);
char					*get_path(char *str, t_env *env);
char					get_escape(char c);
char					*extract_word(char *line, int *i, t_env *env);
// token_heredoc.c
t_token					*initialized_delimiter(t_token *token, t_env *env);
char					*heredoc_quote_check(char *result, char *line, int *i,
							t_env *env);
void					parse_delimiter(t_token *token, char *line, int *i,
							t_env *env);
void					heredoc_delimiter(t_token *token, char *line, int *i,
							t_env *env);
// token_Expansion.c
char					*expand_env(char *line, int *i, t_env *env);
char					*variable_expansion(char *line, int *i, t_env *env);
char					*quote_expansion(char *line, int *i, char quote_str,
							t_env *env);
char					*expand_quote_cond(char *line, int *i, char quote_str);
char					*expand_exit_status(int *i, t_env *env);
// handle_tokens3.c
char					*handle_ansi_c(char *result, char *line, int *i,
							t_env *env);
char					*handle_backslash(char *result, char *line, int *i,
							t_env *env);
char					*handle_tilde(char *line, int *i, t_env *env);
// handle_tokens2.c
char					*handle_single_quote(char *result, char *line, int *i,
							t_env *env);
char					*handle_double_quote(char *result, char *line, int *i,
							t_env *env);
char					*handle_env_var(char *result, char *line, int *i,
							t_env *env);
char					*handle_tilde_word(char *result, char *line, int *i,
							t_env *env);
// handle_tokens1.c
char					*process_dollar(char *result, char *line, int *i,
							t_env *env);
char					*process_quotes(char *result, char *line, int *i,
							t_env *env);
char					*process_escape_chars(char *result, char *line, int *i,
							t_env *env);
char					*process_special_char(char *result, char *line, int *i,
							t_env *env);
char					*process_word_token(char *result, char *line, int *i,
							t_env *env);
// handle_tokens.c
void					handle_pipe(t_token *token, int *i, t_env *env);
void					handle_redirect_in(t_token *token, char *line, int *i,
							t_env *env);
void					handle_redirect_out(t_token *token, char *line, int *i,
							t_env *env);
void					handle_semic(t_token *token, int *i, t_env *env);
void					handle_word(t_token *token, char *line, int *i,
							t_env *env);

/*garbage_callector*/
t_gc					*gc_new(void);
void					*my_malloc(t_gc *gc, int size);
void					gc_register(t_gc *gc, void *ptr);
void					gc_unregister(t_gc *gc, void *ptr);
void					gc_free_all(t_gc *gc);

// validate_syntax.c
int						print_syntax_message(char *str, t_env *env);
int						validate_redirect_syntax(t_token *tokens, t_env *env);
int						validate_pipe_syntax(t_token *tokens, t_env *env,
							int check);
int						validate_syntax(t_token *tokens, t_env *env);

#endif