/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jainavas <jainavas@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/29 14:21:20 by mpenas-z          #+#    #+#             */
/*   Updated: 2025/01/08 19:50:26 by jainavas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/mini.h"

// This function iterates the list executing each command.
// If a command fails, output error and keep going.
// For some reason, in bash echo after a failed pipe prints before error.
// -> Should we store cmd outputs and then evaluate priority?
int	run_cmd_list(t_mini *mini, t_cmd **head)
{
	t_cmd	*curr;
	int		fdret;
	int		fdret2;

	curr = *head;
	fdret = 0;
	fdret2 = 0;
	while (curr)
	{
		if (curr->infile)
		{
			if (access(curr->infile, O_RDONLY) == -1)
			{
				curr = curr->next;
				ft_putstr_fd("Access denied\n", 2);
				continue ;
			}
			fdret2 = open(curr->infile, O_RDONLY);
			if (fdret2 == -1)
				break ;
			else
				fdret = fdret2;
		}
		if (curr->lim)
			fdret = dolimitator(curr->lim, mini);
		fdret = execute_command(mini, curr, fdret);
		if (curr->lim)
			fileunlinker("tmp_heredoc");
		if (*curr->outfiles)
			fdtomfiles(curr->outfiles, fdret);
		curr = curr->next;
	}
	if (fdret2 == -1)
		return (ft_putstr_fd("bash: ", 1), ft_putstr_fd(curr->infile, 1), ft_putstr_fd(" does not exist\n", 1), 127);
	curr = *head;
	while (curr->next)
	{
		waitpid(curr->pid, &curr->pidstatus, 0);
		curr = curr->next;
	}
	waitpid(curr->pid, &curr->pidstatus, 0);
	if ((cmdcount(head) == 1 && (*head)->isbltin == 1))
		fdtofd(cmdlast(*head)->fd[READ_FD], STDOUT_FILENO);
	return (0);
}

// This function executes the current command once all pipes, 
// files and redirections have been set up.
int	execute_command(t_mini *mini, t_cmd *cmd, int infd)
{
	int	tmpfd;

	if (isbuiltin(cmd))
		return (builtins(mini, cmd));
	if (!cmdexistence(cmd->cmd, mini))
		return (ft_putstr_fd(cmd->cmd, 1), ft_putstr_fd(": not exists\n", 1), 0);
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
