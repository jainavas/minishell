/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmdlisthandle.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jainavas <jainavas@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/07 16:14:33 by jainavas          #+#    #+#             */
/*   Updated: 2025/01/12 22:56:34 by jainavas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

void	cmdadd_back(t_cmd **lst, t_cmd *new)
{
	t_cmd	*tmp;

	if (*lst == NULL)
		*lst = new;
	else
	{
		tmp = cmdlast(*lst);
		new->next = NULL;
		tmp->next = new;
		new->prev = tmp;
	}
}

t_cmd	*cmdlast(t_cmd *lst)
{
	t_cmd	*tmp;

	tmp = lst;
	if (tmp)
		while (tmp->next)
			tmp = tmp->next;
	return (tmp);
}

t_fout	*outfilelast(t_fout *lst)
{
	t_fout	*tmp;

	tmp = lst;
	if (tmp)
		while (tmp->next)
			tmp = tmp->next;
	return (tmp);
}

t_fout	*outfilesearchbyfile(char *file, t_fout *lst)
{
	t_fout	*tmp;

	tmp = lst;
	while (tmp && tmp->next)
	{
		if (!ft_strcmpspace(file, tmp->file))
			return (tmp);
		tmp = tmp->next;
	}
	return (NULL);
}

int	outfcount(t_fout **head)
{
	t_fout	*tmp;
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
