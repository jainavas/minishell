/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mpenas-z <mpenas-z@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/06 17:45:35 by mpenas-z          #+#    #+#             */
/*   Updated: 2024/12/06 23:08:28 by mpenas-z         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini.h"

int	g_signal;

void	set_signals(void)
{
	t_sig	sa_sigint;
	t_sig	sa_sigquit;
	
	g_signal = 0;
	sa_sigint.sa_handler = &handle_sigint;
	sigaction(SIGINT, &sa_sigint, NULL);
	sa_sigquit.sa_handler = &handle_sigquit;
	sigaction(SIGQUIT, &sa_sigquit, NULL);
}

void	handle_sigint(int sig)
{
	g_signal = sig;
	rl_replace_line("", 0);
	rl_on_new_line();
	ft_putstr_fd("\n", 2);
}

void	handle_sigquit(int sig)
{
	g_signal = sig;
}
