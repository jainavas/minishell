/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minish2.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jainavas <jainavas@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/10 15:56:12 by jainavas          #+#    #+#             */
/*   Updated: 2024/11/15 17:52:12 by jainavas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini.h"

int	recread(t_mini *mini)
{
	char	**buf;
	char	*buf2;
	int		fdin;
	int		pid;

	write(1, "minishell% ", 12);
	fdin = 0;
	buf2 = get_next_line(0);
	buf = NULL;
	buf2[ft_strlen(buf2) - 1] = '\0';
	if (checkkill(buf2))
		return (free(buf2), 1);
	if (ft_strncmp("cd ", buf2, 3) == 0)
		return (docd(&buf2[3]), free(buf2), 0);
	debuginout(buf2, mini);
	if ((ft_strchr(buf2, '<') != NULL && ft_strchr(buf2, '<')[1] == '<') && ft_strchr(buf2, '|') != NULL)
	{
		buf = preppipexlim(buf2);
		pid = fork();
		if (pid == 0)
		{
			pipex(((ft_strcount(buf2, '|') + 1) + 4), buf, mini->envp, mini);
			free(mini->fileout);
			free(mini->infile);
			return (free(buf2), freedoublepointer(buf), 1);
		}
		else
		{
			wait(NULL);
			free(mini->fileout);
			free(mini->infile);
			return (free(buf2), freedoublepointer(buf), 0);
		}
	}
	else if (ft_strchr(buf2, '|') != NULL)
	{
		buf = preppipex(buf2, mini->infile, mini->fileout);
		pid = fork();
		if (pid == 0)
		{
			pipex(((ft_strcount(buf2, '|') + 1) + 3), buf, mini->envp, mini);
			return (free(buf2), freedoublepointer(buf), 1);
		}
		else
		{
			wait(NULL);
			return (free(buf2), freedoublepointer(buf), 0);
		}
	}
	else
	{
		buf2 = debugbuffer(buf2);
		buf = ft_split(buf2, ' ');
		free(buf2);
		buf2 = pathseek(buf, mini->envp);
		if (mini->infile)
			fdin = open(mini->infile, O_RDONLY);
		if (!buf2)
			return (write(1, "Unknown command\n", 16), free(mini->infile), free(mini->fileout), freedoublepointer(buf), 0);
		alonecmdcall(fdin, buf, pathseek(buf, mini->envp), mini);
		free(mini->fileout);
		free(mini->infile);
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

	mini->infile = NULL;
	mini->fileout = NULL;
	x = recread(mini);
	while (x == 0)
	{
		mini->infile = NULL;
		mini->fileout = NULL;
		x = recread(mini);
	}
	return (0);
}

char	*debugbuffer(char *buf)
{
	char	*tmp;

	tmp = NULL;
	if (ft_strchr(buf, '<'))
	{
		tmp = ft_substr(buf, ft_strchr(buf, '<') - buf + 2, ft_strlen(buf) - (ft_strchr(buf, '<') - buf + 2));
		free(buf);
		buf = tmp;
	}
	if (ft_strchr(buf, '>'))
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
	if (ft_strchr(buf, '>') == NULL)
	 	tmp2 = ft_strdup("/dev/stdout");
	else
	{
		if (ft_strchr(buf, '>')[1] == '>')
			tmp2 = ft_substr(buf, (ft_strchr(buf, '>') - buf + 3), ft_strlen(buf));
		else
			tmp2 = ft_substr(buf, (ft_strchr(buf, '>') - buf + 2), ft_strlen(buf));
	}
	res[(ft_strcount(buf, '|') + 1) + 3] = tmp2;
	return (res[(ft_strcount(buf, '|') + 1) + 4] = NULL, res);
}
