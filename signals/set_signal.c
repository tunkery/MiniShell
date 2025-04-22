/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   set_signal.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bolcay <bolcay@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/18 12:36:45 by hpehliva          #+#    #+#             */
/*   Updated: 2025/03/23 12:18:21 by bolcay           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void set_signal_heredoc(void)
{
    struct sigaction    sa;

    sa.sa_handler = sigint_handler_command;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_RESTART;
    sigaction(SIGINT, &sa, NULL);
    sa.sa_handler = SIG_IGN;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    sigaction(SIGQUIT, &sa, NULL);
}

void set_signal_pipe(void)
{
    struct sigaction sa_in;
    struct sigaction sa_quit;

    sa_in.sa_handler = SIG_IGN;
    sigemptyset(&sa_in.sa_mask);
    sa_in.sa_flags = SA_RESTART;
    sigaction(SIGINT, &sa_in, NULL);
    sa_quit.sa_handler = SIG_IGN;
    sigemptyset(&sa_quit.sa_mask);
    sa_quit.sa_flags = 0;
    sigaction(SIGINT, &sa_quit, NULL);
    turn_off_echo();

}

void    turn_off_echo(void)
{
    struct termios  term;

    if(tcgetattr(STDIN_FILENO, &term) == -1)
        return ;
    term.c_lflag &= ~(ECHOCTL);
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &term);
}

void    sigint_handler_command(int signo)
{
    (void)signo;
    write(STDOUT_FILENO, "\n", 1);

}

void    signal_mode_command(void)
{
    struct sigaction    sa;

    sa.sa_handler = sigint_handler_command;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_RESTART;
    sigaction(SIGINT, &sa, NULL);
    sa.sa_handler = SIG_IGN;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    sigaction(SIGQUIT, &sa, NULL);
}

void    sigint_handler_read(int signo)
{
    (void)signo;
    write(STDOUT_FILENO, "\n", 1);
    rl_on_new_line();
    rl_replace_line("", 0);
    rl_redisplay();
}


void    signal_mode_read(void)
{
    struct sigaction    sa_input;
    struct sigaction    sa_output;

    sa_input.sa_handler = sigint_handler_read;
    sigemptyset(&sa_input.sa_mask);
    sa_input.sa_flags = SA_RESTART;
    sigaction(SIGINT, &sa_input, NULL);
    sa_output.sa_handler = SIG_IGN;
    sigemptyset(&sa_output.sa_mask);
    sa_output.sa_flags = 0;
    sigaction(SIGQUIT, &sa_output, NULL);
    turn_off_echo();
}