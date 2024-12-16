/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_isalnum.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jainavas <jainavas@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/12 00:52:35 by jainavas          #+#    #+#             */
/*   Updated: 2024/08/12 00:52:35 by jainavas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./libft.h"

int	ft_isalnum(int c)
{
	if (ft_isalpha(c) || ft_isdigit(c))
		return (1);
	return (0);
}

int	ft_isbashprotected(int c)
{
	if (c == '=' || c == '|' || c == '<' || c == '>')
		return (1);
	return (0);
}

int	ft_isgroup(char	*tolook, int group(int))
{
	int	i;

	i = -1;
	while (tolook[++i])
		if (group(tolook[i]) == 1)
			return (1);
	return (0);
}
