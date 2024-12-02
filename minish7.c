/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minish7.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jainavas <jainavas@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/30 16:38:08 by jainavas          #+#    #+#             */
/*   Updated: 2024/12/02 16:50:04 by jainavas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini.h"

int	counttmps(t_envar **lst)
{
	t_envar	*new;
	int		i;

	new = *lst;
	i = 0;
	while (new != NULL)
	{
		if (strncmp("holatmp_", new->name, 8) == 0)
			i++;
		new = new->next;
	}
	return (i);
}

void	dpcheckenvars(char **buf, t_mini *mini)
{
	int	i;

	i = -1;
	while (buf[++i])
		buf[i] = checkenvvars(buf[i], mini);
}

