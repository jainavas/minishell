/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minish3.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jainavas <jainavas@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/12 21:06:20 by jainavas          #+#    #+#             */
/*   Updated: 2024/11/16 20:19:06 by jainavas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini.h"

void	docd(char *path)
{
	if (access(path, F_OK) == 0)
		chdir(path);
	else
		printf("cd: no such file or directory: %s\n", path);
}

void	debuginout(char *buf2, t_mini *mini)
{
	if (ft_strchr(buf2, '<') != NULL && ft_strchr(buf2, '<')[1] != '<')
		mini->infile = ft_substr(buf2, 0, ft_strchr(buf2, '<') - buf2 - 1);
	else
		mini->infile = ft_strdup("/dev/stdin");
	if (ft_strchr(buf2, '>') != NULL)
		mini->fileout = ft_substr(buf2, ft_strchr(buf2, '>') - buf2 + 2, ft_strlen(buf2) - (ft_strchr(buf2, '>') - buf2));
	else
		mini->fileout = ft_strdup("/dev/stdout");
	mini->out = 0;
	if (ft_strchr(buf2, '>') != NULL)
		mini->out = 1;
	mini->appendout = 0;
	if (ft_strchr(buf2, '>') != NULL && ft_strchr(buf2, '>')[1] == '>')
		mini->appendout = 1;
}

int ft_dstrchr(char **s, char *s2)
{
	int	i;
	int	j;

	i = -1;
	if (!s || !s2)
		return (-1);
	while (s[++i])
	{
		j = -1;
		if (ft_strncmp(s[i], s2, ft_strlen(s[i])) == 0)
				return (i);
	}
	return (-1);
}

int ft_dstrlen(char **s)
{
	int	i;

	i = 0;
	if (!s)
		return (0);
	while (s[i])
		i++;
	return (i);
}

char	**optionshandle(char **buf, t_mini *mini)
{
	char	*buf2;
	int		i;

	i = -1;
	while (buf[++i] && !buf2 && i < ft_dstrlen(buf))
		buf2 = pathseek(&buf[i], mini->envp);
	if (i >= ft_dstrlen(buf) - 1)
		return (buf);
	free(buf2);
	buf2 = pathseek(&buf[++i], mini->envp);
	if (!buf2)
	{
		buf2 = ft_strjoin(buf[i - 1], " ");
		buf2 = ft_strjoin(buf2, buf[i]);
		free(buf[i - 1]);
		buf[i - 1] = buf2;

	}
}
