/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minish2.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jainavas <jainavas@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/10 15:56:12 by jainavas          #+#    #+#             */
/*   Updated: 2024/12/02 17:52:52 by jainavas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini.h"

int	recread(t_mini *mini)
{
	char	**buf;
	char	*buf2;

	buf2 = readline("minishell% ");
	if (buf2[0] == '\0')
		return (free(buf2), 0);
	add_history(buf2);
	if (ft_strchrtwo(buf2, '"', 39))
	{
		if (checkquotes(buf2, mini) == -1)
			return (write(1, "Error\n", 6), free(buf2),
					free(mini->quotesbuf), mini->quotesbuf = NULL, 0);
		free(buf2);
		buf2 = ft_strdup(mini->quotesbuf);
		free(mini->quotesbuf);
		mini->quotesbuf = NULL;
	}
	else
		buf2 = checkenvvars(buf2, mini);
	buf = NULL;
	if (checkkill(buf2))
		return (free(buf2), rl_clear_history(), 1);
	if (ft_strncmp("cd ", buf2, 3) == 0)
		return (buf2 = checkenvvars(buf2, mini), docd(&buf2[3]), free(buf2), 0);
	if (ft_strncmp("echo ", buf2, 5) == 0)
		return (buf2 = checkenvvars(buf2, mini), doecho(buf2), 0);
	if (ft_strchr(buf2, '=') && ft_strchr(buf2, '=')[-1] != ' '
		&& ft_strchr(buf2, '=')[1] != ' ')
	{
		buf2 = checkenvvars(buf2, mini);
		entvars(mini->envars, ft_substr(buf2, 0,
			ft_strchr(buf2, '=') - buf2), ft_strdup(ft_strchr(buf2, '=') + 1));
		return (free(buf2), 0);
	}
	buf2 = debuginout(buf2, mini);
	if (checkinfile(mini))
		return (free(buf2), free(mini->infile), 0);
	buf = ft_splitchars(buf2, "<|");
	dpcheckenvars(buf, mini);
	if ((ft_strchr(buf2, '<') != NULL && ft_strchr(buf2, '<')[1] == '<') &&
		ft_strchr(buf2, '|') != NULL)
		return (dolimwithpipe(buf2, buf, mini));
	else if (ft_strchr(buf2, '|') != NULL)
		return (dopipes(buf2, buf, mini));
	else
		return (docmd(buf2, buf, mini));
	return (free(buf2), 0);
}

int	checkinfile(t_mini *mini)
{
	if (access(mini->infile, R_OK) != 0)
		return (ft_putstr_fd("minishell: ", 1), ft_putstr_fd(mini->infile, 1),
				ft_putendl_fd(": No existe el archivo o el directorio", 1), 1);
	return (0);
}

int	recursiva(t_mini *mini)
{
	int	x;

	mini->infile = NULL;
	x = recread(mini);
	while (x == 0)
	{
		mini->infile = NULL;
		x = recread(mini);
	}
	return (0);
}

char	**preppipexlim(char *buf, char **antbuf)
{
	char	**res;
	int		i;

	i = -1;
	res = ft_calloc((ft_strcount(buf, '|') + 1) + 4, sizeof(char *));
	res[0] = ft_strdup("a");
	res[1] = ft_strdup("here_doc");
	res[2] = antbuf[1];
	res[3] = antbuf[0];
	if (ft_strcount(buf, '|') == 1)
		res[4] = antbuf[2];
	else
	{
		while (++i + 1 < ft_dstrlen(antbuf))
			res[i + 4] = antbuf[i + 2];
	}
	res[(ft_strcount(buf, '|') + 1) + 3] = NULL;
	free(antbuf);
	return (res);
}
