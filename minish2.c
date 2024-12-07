/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minish2.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jainavas <jainavas@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/10 15:56:12 by jainavas          #+#    #+#             */
/*   Updated: 2024/12/07 17:45:52 by mpenas-z         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini.h"

int	recread(t_mini *mini)
{
	char	**buf;
	char	*buf2;
	int		t;

	buf2 = readline("minishell% ");
	if (!buf2)
		return (1);
	buf2 = initialdebug(mini, buf2);
	if (!buf2)
		return (0);
	buf = NULL;
	t = builtins(mini, buf2);
	if (t != -1)
		return (g_status = t, t);
	buf2 = debuginout(buf2, mini);
	if (checkinfile(mini))
		return (g_status = 127, free(buf2), free(mini->infile), 0);
	buf = ft_splitchars(buf2, "<|");
	dpcheckenvars(buf, mini);
	g_status = exec(mini, buf2, buf);
	return (0);
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
