/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstsize.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jainavas <jainavas@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/16 20:12:15 by jainavas          #+#    #+#             */
/*   Updated: 2024/09/16 20:23:09 by jainavas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./libft.h"

int	ft_lstsize(t_list *lst)
{
	t_list	*new;
	int		i;

	new = lst;
	i = 0;
	while (new)
	{
		new = new->next;
		i++;
	}
	return (i);
}
