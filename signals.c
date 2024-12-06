/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mpenas-z <mpenas-z@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/06 17:45:35 by mpenas-z          #+#    #+#             */
/*   Updated: 2024/12/06 20:24:12 by mpenas-z         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini.h"

volatile int	global_signal;

void	set_signals(void)
{
	t_sig	sa_sigint;
	t_sig	sa_sigquit;
	
	global_signal = 0;
	sa_sigint.sa_handler = &handle_sigint;
	sigaction(SIGINT, &sa_sigint, NULL);
	sa_sigquit.sa_handler = &handle_sigquit;
	sigaction(SIGQUIT, &sa_sigquit, NULL);
}

void	handle_sigint(int sig)
{
	global_signal = sig;
	ft_putstr_fd("\n", 2);
	// signal(SIGINT, SIG_DFL);
}

void	handle_sigquit(int sig)
{
	global_signal = sig;
	// signal(SIGQUIT, SIG_DFL);
}
