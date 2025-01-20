/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins4.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jainavas <jainavas@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/20 19:56:18 by jainavas          #+#    #+#             */
/*   Updated: 2025/01/20 20:01:08 by jainavas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

int	doexit(t_cmd *cmd)
{
	if (cmd->argc > 2)
		return (ft_putstr_fd("exit: too many arguments\n", 2), exit(1), 1);
	if (cmd->argv[1] && checkovrfandchar(cmd->argv[1]))
		return (exit(ft_atoi(cmd->argv[1])), 0);
	if (cmd->argv[1])
		return (ft_putstr_fd("exit: incorrect arguments\n", 2), exit(2), 1);
	else
		return (exit(0), 0);
}

int	donewvarent(t_cmd *cmd, t_mini *mini)
{
	cmd->cmd = checkenvvars(cmd->cmd, mini);
	entvars(&mini->env,
		ft_strndup(cmd->cmd, ft_strchr(cmd->cmd, '=') - cmd->cmd),
		ft_strdup(ft_strchr(cmd->cmd, '=') + 1));
	return (close(cmd->fd[READ_FD]), close(cmd->fd[WRITE_FD]), 0);
}
