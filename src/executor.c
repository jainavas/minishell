/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jainavas <jainavas@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/29 14:21:20 by mpenas-z          #+#    #+#             */
/*   Updated: 2025/01/06 03:32:25 by jainavas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/mini.h"

// This function iterates the list executing each command.
// If a command fails, output error and keep going.
// For some reason, in bash echo after a failed pipe prints before error.
// -> Should we store cmd outputs and then evaluate priority?
int	run_cmd_list(t_mini *mini, t_list *head)
{
	t_list	*curr;
	t_cmd	*cmd;
	int		fdret;
	char	buf;

	curr = head;
	fdret = 0;
	while (curr)
	{
		// if (((t_cmd *)curr->content)->infile)
		// {
		// 	fdret2 = open(((t_cmd *)curr->content)->infile, O_RDONLY);
		// 	if (fdret2 == -1)
		// 		break ;
		// 	else
		// 		fdret = fdret2;
		// }
		cmd = (t_cmd *)curr->content;
		fdret = execute_command(mini, cmd, fdret);
		if (*cmd->outfiles)
			fdtomfiles(cmd->outfiles, fdret);
		curr = curr->next;
	}
	// if (fdret2 == -1)
	// 	return (ft_putstr_fd("bash: ", 1), ft_putstr_fd(cmd->infile, 1), ft_putstr_fd(" does not exist\n", 1), 0);
	if (fdret != 0 && read(fdret, &buf, 0) != -1)
		fdtofd(fdret, STDOUT_FILENO);
	return (0);
}

// This function executes the current command once all pipes, 
// files and redirections have been set up.
int	execute_command(t_mini *mini, t_cmd *cmd, int infd)
{
	int	tmpfd;

	tmpfd = builtins(mini, cmd);
	if (tmpfd != -2)
		return (g_status = 0, tmpfd);
	if (!cmdexistence(cmd->cmd, mini))
		return (ft_putstr_fd(cmd->cmd, 1), ft_putstr_fd(": not exists\n", 1), 0);
	tmpfd = alonecmdcall(infd, cmd->argv, pathseekenv(&cmd->cmd, envtodoublechar(mini->env)), mini);
	return (tmpfd);
}

int	cmdcount(t_list **head)
{
	t_list	*tmp;
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
