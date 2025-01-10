/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jainavas <jainavas@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/06 17:45:35 by mpenas-z          #+#    #+#             */
/*   Updated: 2025/01/10 18:54:33 by jainavas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	set_signals(void)
{
	t_sig	sa_sigint;

	sa_sigint.sa_handler = &handle_sigint;
	sigaction(SIGINT, &sa_sigint, NULL);
	signal(SIGQUIT, SIG_IGN);
}

void	handle_sigint(int sig)
{
	(void)sig;
	g_status = 130;
	new_prompt();
}

void	new_prompt(void)
{
	ft_putstr_fd("\n", 2);
	rl_replace_line("", 0);
	rl_on_new_line();
	rl_redisplay();
}
