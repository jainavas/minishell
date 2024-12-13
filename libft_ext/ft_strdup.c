/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strdup.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jainavas <jainavas@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/12 15:27:49 by jainavas          #+#    #+#             */
/*   Updated: 2024/08/12 15:27:49 by jainavas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strdup(const char *s)
{
	char	*res;

	if (!s)
		return ("");
	res = malloc((ft_strlen(s) + 1) * sizeof(char));
	if (res == NULL)
		return (NULL);
	ft_strlcpy(res, s, ft_strlen(s) + 1);
	return (res);
}

char	*ft_strfilterdup(const char *s, char filter)
{
	char	*res;
	int		i;
	int		j;

	i = -1;
	j = -1;
	res = malloc((ft_strlen(s) - ft_strcount(s, filter) + 1) * sizeof(char));
	if (res == NULL || s == NULL)
		return (ft_strdup(""));
	while (s[++i])
		if (s[i] != filter)
			res[++j] = s[i];
	if (j == -1)
		return (ft_strdup(""));
	return (res);
}

char	*ft_strinsertdup(char *s, char *var, char *content)
{
	char	*res;
	int		i;
	int		j;

	i = -1;
	j = -1;
	res = NULL;
	res = malloc((ft_strlen(s) - (ft_strlen(var))
				+ ft_strlen(content) + 1) * sizeof(char));
	if (res == NULL || s == NULL)
		return (ft_strdup(""));
	while (s[++i] && s[i] != '$')
		res[i] = s[i];
	while (content[++j])
		res[i++] = content[j];
	while (s[i + ft_strlen(var) - j])
	{
		res[i] = s[i + ft_strlen(var) - j];
		i++;
	}
	res[i] = '\0';
	if (j == 0)
		return (ft_strdup(""));
	return (free(s), res);
}
