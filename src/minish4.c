/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minish4.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jainavas <jainavas@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/19 20:05:57 by jainavas          #+#    #+#             */
/*   Updated: 2024/12/07 17:17:08 by mpenas-z         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini.h"

int	dolimwithpipe(char *buf2, char **buf, t_mini *mini)
{
	int	status;

	buf2 = checkenvvars(buf2, mini);
	buf = preppipexlim(buf2, buf);
	status = pipex(((ft_strcount(buf2, '|') + 1) + 4), buf, mini->envp, mini);
	free(mini->infile);
	return (free(buf2), freedoublepointer(buf), status);
}

int	dopipes(char *buf2, char **buf, t_mini *mini)
{
	int	status;
	
	buf2 = checkenvvars(buf2, mini);
	buf = preppipex(buf2, mini->infile, buf);
	status = pipex(((ft_strcount(buf2, '|') + 1) + 2), buf, mini->envp, mini);
	return (free(buf2), freedoublepointer(buf), status);
}

int	docmd(char *buf2, char **buf, t_mini *mini)
{
	int		fdin;
	char	**aux;
	int		status;

	if (ft_strcount(buf2, '<') == 2)
		return (free(buf2), dolimitonecmd(buf, mini));
	fdin = open(mini->infile, O_RDONLY);
	buf2 = checkenvvars(buf2, mini);
	aux = ft_split(buf2, ' ');
	free(buf2);
	if (!aux)
		return (write(1, "Unknown command\n", 16), free(mini->infile),
			freedoublepointer(buf), 127);
	buf2 = pathseek(aux, mini->envp);
	if (!buf2)
		return (write(1, "Unknown command\n", 16), free(mini->infile),
			freedoublepointer(buf),
			freedoublepointer(aux), free(buf2), 127);
	status = alonecmdcall(fdin, aux, pathseek(aux, mini->envp), mini);
	return (free(mini->infile),
		freedoublepointer(buf), free(buf2), freedoublepointer(aux), status);
}

int	checkquotes(char *buf, t_mini *mini)
{
	char		*tmp;
	char		*tmp2;
	char		*aux;

	if (!buf)
		return (0);
	tmp = ft_strchrtwo(buf, '"', 39);
	aux = checkenvvars(ft_substr(buf, 0, tmp - buf), mini);
	mini->quotesbuf = ft_strjoin_gnl(mini->quotesbuf, aux);
	free(aux);
	if (tmp && tmp[0] == 39)
	{
		tmp2 = simplequote(mini, buf, tmp);
		if (tmp2 == NULL)
			return (-1);
		return (checkquotes(++tmp2, mini));
	}
	if (tmp && tmp[0] == '"')
	{
		tmp2 = doublequote(mini, buf, tmp);
		if (tmp2 == NULL)
			return (-1);
		return (checkquotes(++tmp2, mini));
	}
	return (1);
}
