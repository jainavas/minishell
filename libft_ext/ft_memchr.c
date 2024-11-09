/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jainavas <jainavas@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/12 01:44:26 by jainavas          #+#    #+#             */
/*   Updated: 2024/08/12 01:44:26 by jainavas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memchr(const void *s, int c, size_t n)
{
	int		i;
	void	*h;

	i = 0;
	while (i < (int)n)
	{
		if (*(char *)(s + i) == c)
		{
			h = (char *)(s + i);
			return (h);
		}
		i++;
	}
	return (NULL);
}
