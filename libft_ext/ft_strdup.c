/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strdup.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jainavas <jainavas@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/12 15:27:49 by jainavas          #+#    #+#             */
/*   Updated: 2024/12/25 11:45:57 by mpenas-z         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strdup(const char *s)
{
	char	*res;

	if (!s)
		return (ft_strdup(""));
	res = malloc((ft_strlen(s) + 1) * sizeof(char));
	if (res == NULL)
		return (NULL);
	ft_strlcpy(res, s, ft_strlen(s) + 1);
	return (res);
}

char	*ft_strndup(const char *s, size_t n)
{
	char	*res;

	if (!s || n <= 0)
		return (ft_strdup(""));
	if (n >= ft_strlen(s))
		n = ft_strlen(s);
	res = malloc((n + 1) * sizeof(char));
	if (res == NULL)
		return (NULL);
	ft_strlcpy(res, s, n + 1);
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

char	*ft_strinsertdup(char *s, char *var, char *content, char flag)
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
	while (s[++i] && s[i] != flag)
		res[i] = s[i];
	while (content[++j])
		res[i++] = content[j];
	while (s[i + ft_strlen(var) - (j - 1)])
	{
		res[i] = s[i + ft_strlen(var) - (j - 1)];
		i++;
	}
	res[i] = '\0';
	if (j == 0)
		return (ft_strdup(""));
	return (free(s), res);
}
