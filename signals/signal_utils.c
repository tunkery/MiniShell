/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bolcay <bolcay@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/24 14:48:30 by bolcay            #+#    #+#             */
/*   Updated: 2025/04/24 14:48:55 by bolcay           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	sigint_handler_read(int signo)
{
	(void)signo;
	write(STDOUT_FILENO, "\n", 1);
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
}

void	signal_mode_read(void)
{
	struct sigaction	sa_input;
	struct sigaction	sa_output;

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
