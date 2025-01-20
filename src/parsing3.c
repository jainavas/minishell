/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing3.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jainavas <jainavas@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/20 19:02:43 by jainavas          #+#    #+#             */
/*   Updated: 2025/01/20 19:03:05 by jainavas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

char	*replace_content(char *str, char *content, char *start, int len)
{
	char	*temp;

	temp = ft_calloc(ft_strlen(str) - ft_strlen(start) + 1, sizeof(char));
	ft_strlcpy(temp, str, ft_strlen(str) - ft_strlen(start) + 1);
	if (content != NULL)
		temp = ft_strjoin_gnl(temp, content);
	if (start[len])
		temp = ft_strjoin_gnl(temp, start + len);
	free (str);
	return (temp);
}

int	get_namelen(char *name)
{
	int	i;

	if (!name)
		return (0);
	if (name[1] == '?')
		return (2);
	i = 1;
	while (name[i] && (isalnum(name[i]) || name[i] == '_'))
		i++;
	return (i);
}

char	**cleannulls(char **prev)
{
	char	**res;
	int		ct;
	int		ct2;

	ct = 0;
	ct2 = 0;
	while (prev[ct] != NULL)
	{
		if (prev[ct][0] != '\0')
			ct2++;
		ct++;
	}
	ct = 0;
	res = ft_calloc(ct2 + 1, sizeof(char *));
	ct2 = 0;
	while (prev[ct] != NULL)
	{
		if (prev[ct][0] != '\0')
			res[ct2++] = ft_strdup(prev[ct]);
		ct++;
	}
	res[ct2] = NULL;
	freedoublepointer(prev);
	return (res);
}
