/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minish2.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jainavas <jainavas@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/10 15:56:12 by jainavas          #+#    #+#             */
/*   Updated: 2024/11/12 21:44:53 by jainavas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini.h"

int	recread(t_mini *mini)
{
	char	**buf;
	char	*buf2;
	int		fdin;

	write(1, "minishell% ", 12);
	fdin = 0;
	buf2 = get_next_line(0);
	buf2[ft_strlen(buf2) - 1] = '\0';
	if (checkkill(buf2))
		return (free(buf2), 1);
	if (ft_strncmp("cd ", buf2, 3) == 0)
		return (docd(&buf2[3]), free(buf2), 0);
	if ((ft_strchr(buf2, '<') != NULL && ft_strchr(buf2, '<')[1] == '<') && ft_strchr(buf2, '|') != NULL)
	{
		buf = preppipexlim(buf2);
		pipex(((ft_strcount(buf2, '|') + 1) + 4), buf, mini->envp);
		return (free(buf2), freedoublepointer(buf), 0);
	}
	if (ft_strchr(buf2, '<') != NULL && ft_strchr(buf2, '<')[1] != '<')
		mini->infile = ft_substr(buf2, 0, ft_strchr(buf2, '<') - buf2 - 1);
	if (ft_strchr(buf2, '>') != NULL)
		mini->fileout = ft_substr(buf2, ft_strchr(buf2, '>') - buf2 + 2, ft_strlen(buf2) - (ft_strchr(buf2, '>') - buf2));
	mini->appendout = 0;
	if (ft_strchr(buf2, '>') != NULL && ft_strchr(buf2, '>')[1] == '>')
		mini->appendout = 1;
	if (ft_strchr(buf2, '|') != NULL)
	{
		buf = preppipex(buf2, mini->infile, mini->fileout);
		pipex(((ft_strcount(buf2, '|') + 1) + 3), buf, mini->envp);
		freedoublepointer(buf);
	}
	else
	{
		buf = ft_split(debugbuffer(buf2, mini), ' ');
		fdin = 0;
		if (mini->infile)
			fdin = open(mini->infile, O_RDONLY);
		alonecmdcall(fdin, buf, pathseek(buf, mini->envp), mini);
	}
	return (free(buf2), 0);
}

int	diffindex(char *buf, char *a, char *b)
{
	return ((b - buf) - (a - buf));
}

int	recursiva(t_mini *mini)
{
	int	x;

	x = 0;
	while (x == 0)
		x = recread(mini);
	return (0);
}

char	*debugbuffer(char *buf, t_mini *mini)
{
	char	*tmp;

	tmp = NULL;
	if (!mini->infile)
		mini->infile = ft_strdup("/dev/stdin");
	else
	{
		tmp = ft_substr(buf, ft_strchr(buf, '<') - buf + 2, ft_strlen(buf) - (ft_strchr(buf, '<') - buf + 2));
		free(buf);
		buf = tmp;
	}
	if (!mini->fileout)
		mini->fileout = ft_strdup("/dev/stdout");
	else
	{
		tmp = ft_substr(buf, 0, ft_strlen(buf) - (ft_strlen(buf) - (ft_strchr(buf, '>') - buf - 1)));
		free(buf);
		buf = tmp;
	}
	return (buf);
}

char **preppipexlim(char *buf)
{
	char	**res;
	char	**tmp;
	char	*tmp2;
	int		i;

	i = -1;
	res = ft_calloc((ft_strcount(buf, '|') + 1) + 5, sizeof(char *));
	res[0] = ft_strdup("a");
	res[1] = ft_strdup("here_doc");
	res[2] = ft_substr(buf, (ft_strchr(buf, '<') - buf + 3), (ft_strchr(buf, '|') - buf - (ft_strchr(buf, '<') - buf + 4)));
	res[3] = ft_substr(buf, 0, (ft_strchr(buf, '<') - buf - 1));
	if (ft_strcount(buf, '|') == 1)
		res[4] = ft_substr(buf, (ft_strchr(buf, '|') - buf + 1), diffindex(buf, ft_strchr(buf, '|') - 1, ft_strchr(buf, '>') - 3));
	else
	{
		tmp2 = ft_substr(buf, (ft_strchr(buf, '|') - buf + 1), (ft_strchr(buf, '>') - buf - (ft_strchr(buf, '|') - buf) - 1));
		tmp = ft_split(tmp2, '|');
		while (tmp[++i])
			res [i + 4] = tmp[i];
		free(tmp2);
	}
	tmp2 = ft_substr(buf, (ft_strchr(buf, '>') - buf + 3), ft_strlen(buf));
	if (ft_strchr(buf, '>') == NULL)
	 	tmp2 = ft_strdup("/dev/stdout");
	res[(ft_strcount(buf, '|') + 1) + 3] = tmp2;
	return (res[(ft_strcount(buf, '|') + 1) + 4] = NULL, res);
}
