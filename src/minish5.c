/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minish5.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jainavas <jainavas@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/21 22:28:59 by jainavas          #+#    #+#             */
/*   Updated: 2025/01/15 19:12:06 by jainavas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	newfileout(t_fout **head, char *file, int app)
{
	t_fout	*new;

	new = ft_calloc(1, sizeof(t_fout));
	new->file = ft_strtrim(file, " >");
	new->out = 1;
	new->appendout = app;
	new->next = NULL;
	new->prev = NULL;
	if (!*head)
		*head = new;
	else
	{
		(foutlast(*head))->next = new;
		new->prev = foutlast(*head);
	}
	free(file);
}

void	alonecmdcallutils(t_cmd *cmd, int fdin)
{
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
	if (cmd->infile)
		dup2(fdin, cmd->fd[READ_FD]);
	else if (cmd->prev && !*cmd->prev->outfiles)
	{
		dup2(cmd->prev->fd[READ_FD], cmd->fd[READ_FD]);
		close(cmd->prev->fd[READ_FD]);
	}
	else
		dup2(fdin, cmd->fd[READ_FD]);
	if (fdin > 2 && fdin != -1)
		close(fdin);
}

int	spacesindex(const char *str)
{
	int	i;

	i = 0;
	while (str[i] == ' ' || (str[i] >= 9 && str[i] <= 13))
		i++;
	return (i);
}

int	cmdexistence(char *cmd, t_mini *mini)
{
	char	*tmp;
	char	**tmp2;
	DIR		*dir;

	if (!cmd)
		return (ft_putstr_fd(cmd, 2), ft_putstr_fd(": not exists\n", 2), mini->status = 127, -1);
	dir = opendir(cmd);
	tmp2 = envtodoublechar(mini->env);
	tmp = pathseekenv(&cmd, tmp2);
	if (tmp)
		return (free(tmp), freedoublepointer(tmp2), 1);
	else
	{
		if (dir && (cmd[0] == '.' || cmd[0] == '/'))
			return (closedir(dir), ft_putstr_fd(cmd, 2), ft_putstr_fd(": is a directory\n", 2), mini->status = 126, -1);
		return (freedoublepointer(tmp2), ft_putstr_fd(cmd, 2), ft_putstr_fd(": not exists\n", 2), mini->status = 127, -1);
	}
}

void	fdtofd(int fdin, int fdout)
{
	char	*buf;

	buf = NULL;
	if (read(fdin, buf, 0) == -1)
		return ;
	buf = get_next_line(fdin);
	while (buf)
	{
		write(fdout, buf, ft_strlen(buf));
		free(buf);
		buf = get_next_line(fdin);
	}
	if (fdout != -1 && fdout != 0 && fdout != 1 && fdout != 2)
		close(fdout);
	if (fdin != -1)
		close(fdin);
}

t_cmd	*cmdsearchbyfd(int fd, t_cmd **head)
{
	t_cmd	*tmp;

	tmp = *head;
	while (tmp)
	{
		if (tmp->fd[READ_FD] == fd)
			return (tmp);
		tmp = tmp->next;
	}
	return (NULL);
}
