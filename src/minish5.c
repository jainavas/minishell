/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minish5.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jainavas <jainavas@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/21 22:28:59 by jainavas          #+#    #+#             */
/*   Updated: 2025/01/04 18:16:20 by jainavas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini.h"

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

void	alonecmdcallutils(int fd[2], int fdin)
{
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
	dup2(fdin, fd[READ_FD]);
	if (fdin > 2)
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

	if (!cmd)
		return (0);
	tmp = pathseek(&cmd, mini->envp);
	if (tmp)
		return (free(tmp), 1);
	else
		return (0);
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
