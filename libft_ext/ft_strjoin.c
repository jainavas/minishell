/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jainavas <jainavas@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/15 01:56:26 by jainavas          #+#    #+#             */
/*   Updated: 2024/08/15 01:56:26 by jainavas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./libft.h"

char	*ft_strjoin(char *s1, char const *s2)
{
	char	*res;
	int		i;

	if (!s1)
	{
		s1 = malloc(sizeof(char) + 1);
		s1[0] = 0;
	}
	res = malloc(ft_strlen(s1) + ft_strlen(s2) + 1);
	if (res == NULL)
		return (NULL);
	i = 0;
	while (s1[i] != '\0')
	{
		res[i] = s1[i];
		i++;
	}
	i = 0;
	while (s2[i] != '\0')
	{
		res[i + ft_strlen(s1)] = s2[i];
		i++;
	}
	res [i + ft_strlen(s1)] = '\0';
	return (res);
}

// int	main(){
// 	printf("%s \n", ft_strjoin("hola bu","enas tardes"));
// }