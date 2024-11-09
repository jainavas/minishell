/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strmapi.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jainavas <jainavas@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/13 17:29:22 by jainavas          #+#    #+#             */
/*   Updated: 2024/08/13 17:29:22 by jainavas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strmapi(char const *s, char (*f)(unsigned int, char))
{
	int		i;
	char	*res;

	if (!s)
		return (NULL);
	res = malloc(ft_strlen(s) + 1);
	if (res == NULL)
		return (NULL);
	i = 0;
	while (i < (int)ft_strlen(s))
	{
		res[i] = f(i, s[i]);
		i++;
	}
	res[i] = s[i];
	return (res);
}
