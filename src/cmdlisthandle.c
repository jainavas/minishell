/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmdlisthandle.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jainavas <jainavas@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/07 16:14:33 by jainavas          #+#    #+#             */
/*   Updated: 2025/01/10 18:54:33 by jainavas         ###   ########.fr       */
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
