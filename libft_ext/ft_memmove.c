/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memmove.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jainavas <jainavas@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/12 00:58:43 by jainavas          #+#    #+#             */
/*   Updated: 2024/08/12 00:58:43 by jainavas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./libft.h"

void	*ft_memmove(void *dst, const void *src, size_t len)
{
	unsigned char	*d;
	unsigned int	i;

	d = (unsigned char *)dst;
	i = 0;
	if (dst == src || (int)len == 0)
		return (dst);
	if (d < (unsigned char *)src)
	{
		while (len--)
		{
			d[i] = *(char *)(src + i);
			i++;
		}
	}
	else
	{
		i = (int)len - 1;
		while ((int)len--)
		{
			d[i] = *(char *)(src + i);
			i--;
		}
	}
	return (dst);
}
