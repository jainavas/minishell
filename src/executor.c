/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jainavas <jainavas@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/29 14:21:20 by mpenas-z          #+#    #+#             */
/*   Updated: 2025/01/17 18:08:05 by jainavas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

// This function iterates the list executing each command.
// If a command fails, output error and keep going.
// For some reason, in bash echo after a failed pipe prints before error.
// -> Should we store cmd outputs and then evaluate priority?
int	run_cmd_list(t_mini *mini, t_cmd **head)
{
	t_cmd	*curr;
	int		fdret;

	curr = *head;
	fdret = 0;
	while (curr)
	{
		mini->status = 0;
		g_status = 0;
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
	return (closecmdsfd(head), 0);
}

// This function executes the current command once all pipes, 
// files and redirections have been set up.
int	execute_command(t_mini *mini, t_cmd *cmd, int infd)
{
	int	tmpfd;
	int	tmp;

	if (isbuiltin(cmd))
		return (builtins(mini, cmd));
	tmp = cmdexistence(cmd->cmd, mini);
	if (tmp == -1)
		return (-1);
	tmpfd = alonecmdcall(infd, cmd, envtodoublechar(mini->env), mini);
	return (tmpfd);
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

void	fileunlinker(char *file)
{
	char	*tmp;

	tmp = ft_strjoin_gnl(getcwd(NULL, 0), "/");
	tmp = ft_strjoin_gnl(tmp, file);
	unlink(tmp);
	free(tmp);
}

void	closecmdsfd(t_cmd **head)
{
	t_cmd	*tmp;

	tmp = *head;
	while (tmp)
	{
		if (tmp->fd[READ_FD] > 2)
			close(tmp->fd[READ_FD]);
		if (tmp->fd[WRITE_FD] > 2)
			close(tmp->fd[WRITE_FD]);
		tmp = tmp->next;
	}
}

int	selectinflim(t_cmd *cmd, t_mini *mini)
{
	int	fdret;

	fdret = 0;
	if (cmd->priorinflim == 1 && cmd->infile)
	{
		if (!checkpermission(cmd->infile, 1, mini, cmd))
		{
			if (cmd->next)
			{
				pipe(cmd->fd);
				close(cmd->fd[WRITE_FD]);
			}
			if (cmdcount(mini->header) == cmd->cmdn)
				return (-1);
		}
		fdret = open(cmd->infile, O_RDONLY);
		if (fdret == -1)
		{
			if (cmd->next)
			{
				pipe(cmd->fd);
				close(cmd->fd[WRITE_FD]);
			}
			return (-1);
		}
	}
	if (cmd->priorinflim == 2 && cmd->lim)
		fdret = dolimitator(cmd->lim, mini);
	return (fdret);
}
