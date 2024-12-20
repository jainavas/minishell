/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strncmp.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jainavas <jainavas@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/12 01:36:34 by jainavas          #+#    #+#             */
/*   Updated: 2024/08/12 01:36:34 by jainavas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_strncmp(const char *s1, const char *s2, size_t n)
{
	int	i;
	int	res;

	i = 0;
	if (n == 0)
		return (0);
	while (s1[i] == s2[i] && i < (int)n
		&& s1[i] != '\0' && s2[i] != '\0')
		i++;
	if (i == (int)n)
		return (0);
	res = (unsigned char)*(s1 + i) - (unsigned char)*(s2 + i);
	return (res);
}

int	ft_strcmpalnum(const char *s1, const char *s2)
{
	int	i;
	int	res;

	i = 0;
	if (!s1 || !s2)
		return (-1);
	while (s1[i] && s2[i] && s1[i] == s2[i])
		i++;
	if ((!s1[i] && !s2[i]) || (!s1[i] && ft_isalnum(s2[i]) == 0)
		|| (!s2[i] && ft_isalnum(s1[i]) == 0))
		return (0);
	res = (unsigned char)*(s1 + i) - (unsigned char)*(s2 + i);
	return (res);
}

int	ft_strcmpspace(const char *s1, const char *s2)
{
	int	i;
	int	res;

	i = 0;
	if (!s1 || !s2)
		return (-1);
	while (s1[i] && s2[i] && s1[i] == s2[i])
		i++;
	if ((!s1[i] && !s2[i]) || (!s1[i] && s2[i] == ' ')
		|| (!s2[i] && s1[i] == ' '))
		return (0);
	res = (unsigned char)*(s1 + i) - (unsigned char)*(s2 + i);
	return (res);
}
