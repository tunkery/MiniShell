/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   set_signal1.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hpehliva <hpehliva@student.42heilbronn.de  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/28 12:38:47 by hpehliva          #+#    #+#             */
/*   Updated: 2025/04/28 12:38:48 by hpehliva         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	set_signal_heredoc(void)
{
	struct sigaction	sa;

	sa.sa_handler = sigint_handler_command;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = SA_RESTART;
	sigaction(SIGINT, &sa, NULL);
	sa.sa_handler = SIG_IGN;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = 0;
	sigaction(SIGQUIT, &sa, NULL);
}

void	set_signal_pipe(void)
{
	struct sigaction	sa_in;
	struct sigaction	sa_quit;

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
