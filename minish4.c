/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minish4.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jainavas <jainavas@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/19 20:05:57 by jainavas          #+#    #+#             */
/*   Updated: 2024/11/29 18:08:55 by jainavas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini.h"

int	dolimwithpipe(char *buf2, char **buf, t_mini *mini)
{
	buf = preppipexlim(buf2, buf);
	pipex(((ft_strcount(buf2, '|') + 1) + 4), buf, mini->envp, mini);
	free(mini->infile);
	return (free(buf2), freedoublepointer(buf), 0);
}

int	dopipes(char *buf2, char **buf, t_mini *mini)
{
	buf = preppipex(buf2, mini->infile, buf);
	pipex(((ft_strcount(buf2, '|') + 1) + 2), buf, mini->envp, mini);
	return (free(buf2), freedoublepointer(buf), 0);
}

int	docmd(char *buf2, char **buf, t_mini *mini)
{
	int		pid;
	int		fdin;
	char	**aux;

	if (ft_strcount(buf2, '<') == 2)
		return (free(buf2), dolimitonecmd(buf, mini));
	pid = fork();
	if (pid == 0)
	{
		fdin = 0;
		if (ft_strncmp(mini->infile, "/dev/stdin", 10) != 0)
		{
			if (access(mini->infile, R_OK) != 0)
				return (write(1, "wrong file\n", 11), freedoublepointer(buf),
						free(mini->infile), free(buf2), 1);
			fdin = open(mini->infile, O_RDONLY);
			aux = ft_split(buf[1], ' ');
		}
		else
			aux = ft_split(buf[0], ' ');
		free(buf2);
		buf2 = pathseek(aux, mini->envp);
		if (!buf2)
			return (write(1, "Unknown command\n", 16), free(mini->infile),
				freedoublepointer(buf),
				freedoublepointer(aux), free(buf2), 1);
		alonecmdcall(fdin, aux, pathseek(aux, mini->envp), mini);
		return (free(mini->infile),
				freedoublepointer(aux), freedoublepointer(buf), free(buf2), 1);
	}
	wait(NULL);
	return (free(mini->infile),
			freedoublepointer(buf), free(buf2), 0);
}

int	checkquotes(char *buf, t_mini *mini)
{
	char	*tmp;
	char	*tmp2;
	char	*aux;

	if (!buf)
		return (0);
	tmp = ft_strchrtwo(buf, '"', 39);
	aux = checkenvvars(ft_substr(buf, 0, tmp - buf), mini);
	mini->quotesbuf = ft_strjoin_gnl(mini->quotesbuf, aux);
	free(aux);
	if (tmp && tmp[0] == 39)
	{
		tmp2 = ft_strchr(tmp + 1, 39);
		if (!tmp2)
			return (-1);
		tmp = ft_substr(buf, tmp - buf + 1, tmp2 - tmp - 1);
		mini->quotesbuf = ft_strjoin_gnl(mini->quotesbuf, tmp);
		free(tmp);
		return (checkquotes(++tmp2, mini));
	}
	if (tmp && tmp[0] == '"')
	{
		tmp2 = ft_strchr(tmp + 1, '"');
		if (!tmp2)
			return (-1);
		tmp = ft_substr(buf, tmp - buf + 1, tmp2 - tmp - 1);
		mini->quotesbuf = ft_strjoin_gnl(mini->quotesbuf, tmp);
		free(tmp);
		mini->quotesbuf = checkenvvars(mini->quotesbuf, mini);
		return (checkquotes(++tmp2, mini));
	}
	aux = checkenvvars(ft_substr(buf, 0, tmp - buf), mini);
	mini->quotesbuf = ft_strjoin_gnl(mini->quotesbuf, aux);
	free(aux);
	return (1);
}