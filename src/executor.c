/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jainavas <jainavas@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/29 14:21:20 by mpenas-z          #+#    #+#             */
/*   Updated: 2025/01/31 12:26:54 by mpzamora         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

int	run_cmd_list(t_mini *mini, t_cmd **head)
{
	t_cmd	*curr;
	int		fdret;

	curr = *head;
	fdret = 0;
	while (curr)
	{
		mini->status = 0;
		g_signal = -1;
		fdret = selectinflim(curr, mini);
		if (fdret == -1)
		{
			curr = curr->next;
			continue ;
		}
		fdret = execute_command(mini, curr, fdret);
		if (curr->lim)
			fileunlinker("tmp_heredoc");
		if (*curr->outfiles)
			fdtomfiles(curr->outfiles, fdret, mini, curr);
		curr = curr->next;
	}
	postexec(head, mini);
	return (closecmdsfd(head), 0);
}

int	execute_command(t_mini *mini, t_cmd *cmd, int infd)
{
	int	tmpfd;
	int	tmp;

	if (!cmd->cmd)
	{
		if (infd > 2)
			close(infd);
		return (pipe(cmd->fd), close(cmd->fd[WRITE_FD]), infd);
	}
	if (isbuiltin(cmd))
		return (builtins(mini, cmd));
	else if (path_exists(mini, cmd))
	{
		tmp = cmdexistence(cmd->cmd, mini);
		if (tmp == -1)
			return (-1);
		tmpfd = alonecmdcall(infd, cmd, envtodoublechar(mini->env), mini);
		return (tmpfd);
	}
	else
		return (ft_putstr_fd(cmd->cmd, 2),
			ft_putendl_fd(": Not such file or directory", 2),
			mini->status = 127, -1);
}

int	cmdcount(t_cmd **head)
{
	t_cmd	*tmp;
	int		i;

	tmp = *head;
	i = 0;
	while (tmp)
	{
		i++;
		tmp = tmp->next;
	}
	return (i);
}

int	dolimitator(char *lim, t_mini *mini)
{
	int	fd;

	fd = open("tmp_heredoc", O_CREAT | O_WRONLY, 0777);
	limmitator(lim, fd, mini);
	close(fd);
	fd = open("tmp_heredoc", O_RDONLY);
	return (fd);
}

void	postexec(t_cmd **head, t_mini *mini)
{
	t_cmd	*curr;

	curr = *head;
	while (curr->next)
	{
		waitpid(curr->pid, &curr->pidstatus, 0);
		if (WIFEXITED(curr->pidstatus) != 0 && mini->status == 0)
			mini->status = WEXITSTATUS(curr->pidstatus);
		curr = curr->next;
	}
	waitpid(curr->pid, &curr->pidstatus, 0);
	if (WIFEXITED(curr->pidstatus) != 0 && mini->status == 0)
		mini->status = WEXITSTATUS(curr->pidstatus);
	if ((cmdlast(*head)->ifouts == 0) && mini->status == 0)
		fdtofd(cmdlast(*head)->fd[READ_FD], STDOUT_FILENO);
	if (g_signal == 130)
		mini->status = 130;
	g_signal = 0;
}
