/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcat.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jainavas <jainavas@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/12 00:59:54 by jainavas          #+#    #+#             */
/*   Updated: 2024/08/12 00:59:54 by jainavas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./libft.h"

size_t	ft_strlcat(char *dest, const char *src, size_t dstsize)
{
	size_t	i;
	size_t	destlen;
	size_t	srclen;

	i = 0;
	if (dstsize == 0)
		return (ft_strlen(src));
	destlen = ft_strlen(dest);
	srclen = ft_strlen(src);
	if (destlen + 1 > dstsize)
		return (srclen + dstsize);
	else if (destlen + 1 < dstsize)
	{
		while ((src[i] != 0) && (destlen + 1 + i < dstsize))
		{
			dest[destlen + i] = src[i];
			i++;
		}
	}
	dest[destlen + i] = 0;
	return (destlen + srclen);
}
