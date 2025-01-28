/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing3.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jainavas <jainavas@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/20 19:02:43 by jainavas          #+#    #+#             */
/*   Updated: 2025/01/28 16:59:11 by mpenas-z         ###   ########.fr       */
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

void	cases_split_operators(int *i, int *pos, char *buf, char ***split)
{
	if (buf[*i] == '<' && *i + 1 < (int)ft_strlen(buf)
		&& buf[*i + 1] == '<')
	{
		*split[(*pos)++] = ft_strndup(buf + *i, 2);
		*i = *i + 1;
	}
	else if (buf[*i] == '>' && *i + 1 < (int)ft_strlen(buf)
		&& buf[*i + 1] == '>')
	{
		*split[(*pos)++] = ft_strndup(buf + *i, 2);
		*i = *i + 1;
	}
	else
		*split[(*pos)++] = ft_strndup(buf + *i, 1);
}

void	increment_operators(int *amount, int *index, char *buffer)
{
	if (buffer[*index] == '<')
	{
		if (*index + 1 < (int)ft_strlen(buffer) && buffer[*index + 1] == '<')
			*index = *index + 1;
		*amount = *amount + 1;
	}
	else if (buffer[*index] == '>')
	{
		if (*index + 1 < (int)ft_strlen(buffer) && buffer[*index + 1] == '>')
			*index = *index + 1;
		*amount = *amount + 1;
	}
	else
		*amount = *amount + 1;
}
