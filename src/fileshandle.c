/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fileshandle.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jainavas <jainavas@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/21 22:28:59 by jainavas          #+#    #+#             */
/*   Updated: 2025/01/31 11:40:30 by mpzamora         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	anyfdtofile(int fd, t_fout *out, t_cmd *cmd, t_mini *mini)
{
	int		fdo;
	int		r;

	fdo = 1;
	r = checkpermouts(cmd, out->file, mini);
	if (r == 1)
	{
		if (out->appendout == 1)
			fdo = open(out->file, O_WRONLY | O_APPEND);
		else
			fdo = open(out->file, O_WRONLY | O_TRUNC);
	}
	else if (r == -1)
		return (-1);
	else if (out->file)
		fdo = open(out->file, O_RDWR | O_CREAT | O_TRUNC, 0644);
	if (outfcount(cmd->outfiles) == out->foutn && fdo != 1 && fd != -1
		&& cmd->cmd)
		fdtofd(fd, fdo);
	else if (fdo != 1)
		close(fdo);
	return (0);
}

void	fdtomfiles(t_fout **head, int fd, t_mini *mini, t_cmd *cmd)
{
	t_fout	*tmp;

	tmp = *head;
	while (tmp)
	{
		if (tmp->file[0] == '.')
			filesearch(tmp, mini);
		if (tmp->out != -2)
			if (anyfdtofile(fd, tmp, cmd, mini) == -1)
				break ;
		tmp = tmp->next;
	}
	freeoutfiles(head);
}

void	freeoutfiles(t_fout **lst)
{
	t_fout	*new;

	while (*lst != NULL)
	{
		new = *lst;
		*lst = new->next;
		free(new->file);
		free(new);
	}
}

int	filesearch(t_fout *tmp, t_mini *mini)
{
	t_ffdr	*var;
	char	*tmp2;
	char	*tmp3;

	tmp3 = NULL;
	tmp2 = ft_strtrim(tmp->file, " ./");
	if (ft_strchr(tmp2, '/'))
		return (seekcasebar(tmp, tmp2, tmp3, mini));
	else
	{
		var = ft_calloc(1, sizeof(t_ffdr));
		var->directory = getcwd(NULL, 0);
		var->f = ft_strdup(tmp2);
		tmp3 = fileseek(var);
		if (tmp3 == NULL)
			return (ft_putendl_fd("File not found", 2), mini->status = 1, 0);
		else
			return (free(tmp->file), tmp->file = tmp3, 0);
	}
	return (0);
}

t_fout	*foutlast(t_fout *lst)
{
	t_fout	*tmp;

	tmp = lst;
	if (lst)
		while (tmp->next)
			tmp = tmp->next;
	return (tmp);
}
