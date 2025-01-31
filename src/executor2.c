/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor2.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jainavas <jainavas@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/20 18:49:38 by jainavas          #+#    #+#             */
/*   Updated: 2025/01/31 19:00:17 by jainavas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

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
				openoutferrinf(cmd, mini);
			}
			if (cmdcount(mini->header) == cmd->cmdn)
				return (openoutferrinf(cmd, mini), -1);
		}
		fdret = open(cmd->infile, O_RDONLY);
	}
	if (cmd->priorinflim == 2 && cmd->lim)
		fdret = dolimitator(cmd->lim, mini);
	return (fdret);
}

void	openoutferrinf(t_cmd *cmd, t_mini *mini)
{
	t_fout	*tmp;

	if (!*cmd->outfiles)
		return ;
	tmp = *cmd->outfiles;
	while (tmp)
	{
		if (tmp->file[0] == '.')
			filesearch(tmp, mini);
		if (tmp->out != -2 && tmp->priorinfout < cmd->priorinfout)
			if (anyfdtofile(-1, tmp, cmd, mini) == -1)
				break ;
		tmp = tmp->next;
	}
	freeoutfiles(cmd->outfiles);
}

void	set_status(int status, t_mini **mini)
{
	char	*temp;

	(*mini)->status = status;
	temp = ft_itoa(status);
	add_envar((*mini), "?", temp, 2);
	free (temp);
}
