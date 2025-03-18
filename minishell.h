/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bolcay <bolcay@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/10 14:20:41 by bolcay            #+#    #+#             */
/*   Updated: 2025/03/17 12:20:06 by bolcay           ###   ########.fr       */
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
#define PASTEL_BLUE  "\001\033[1;34m"
#define PASTEL_RED   "\001\033[1;31m"

typedef struct s_builtin
{
	const char	*name;
	int		(*foo)(char **args);
}	t_builtin;

typedef struct s_env
{
	char	**envp;
}	t_env;

// Builtin functions

void	cell_exit(char **args);
int		cell_echo(char **args);
void	cell_env(char **env);
void	cell_pwd(void);
void	cell_cd(const char *path);
void	cell_export(char **envp, char *key);
void	cell_unset(char **envp, char *key);

// Env functions

int		key_size(char *str);
int		value_size(char *str);
int		env_size(char **envp);
void	copy_env(char **str, char **envp);
char	**update_env(char **envp, char *key);
char	**get_tokens(char **str);
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


#endif