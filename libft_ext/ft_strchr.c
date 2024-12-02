/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jainavas <jainavas@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/12 01:07:51 by jainavas          #+#    #+#             */
/*   Updated: 2024/08/12 01:07:51 by jainavas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strchr(const char *s, int c)
{
	int	i;

	i = 0;
	while (s[i] != '\0')
	{
		if (s[i] == c)
		{
			return ((char *)&s[i]);
		}
		i++;
	}
	if (c == 0)
		return ((char *)&s[i]);
	return (NULL);
}

char	*ft_revstrchr(const char *s, int c, char first)
{
	int	i;

	i = 0;
	while (s[i] != first)
	{
		if (s[i] == c)
		{
			return ((char *)&s[i]);
		}
		i--;
	}
	if (c == 0)
		return ((char *)&s[i]);
	return (NULL);
}

char	*ft_strchrtwo(const char *s, int c, int k)
{
	int	i;

	i = 0;
	while (s[i] != '\0')
	{
		if (s[i] == c || s[i] == k)
		{
			return ((char *)&s[i]);
		}
		i++;
	}
	if (c == 0 || k == 0)
		return ((char *)&s[i]);
	return (NULL);
}

// #include <stdio.h>
// int main (){
// 	printf("%s", ft_revstrchr(ft_strchr("htrhrha%c rg45g4", '%'), 'h'));
// }