/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minish2.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jainavas <jainavas@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/10 15:56:12 by jainavas          #+#    #+#             */
/*   Updated: 2024/12/25 22:40:44 by mpenas-z         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini.h"

int	recread(t_mini **mini)
{
	char	**buf;
	char	*buf3;
	char	*buf2;
	int		t;

	buf2 = readline("minishell% ");
	if (!buf2)
		return (1);
	buf3 = ft_strdup(buf2);
	t = builtins((*mini), buf2, process_input((*mini), buf3));
	if (t == -1)
		return (t);
	else if (t != -2)
		return (g_status = t, 0);
	buf2 = initialdebug((*mini), buf2);
	if (!buf2)
		return (0);
	buf = NULL;
	buf2 = debuginout(buf2, (*mini));
	if (checkinfile((*mini)))
		return (g_status = 127, free(buf2), free((*mini)->infile), 0);
	buf = ft_splitchars(buf2, "<|");
	dpcheckenvars(buf, (*mini));
	g_status = exec((*mini), buf2, buf);
	return (0);
}

int	checkinfile(t_mini *mini)
{
	if (access(mini->infile, R_OK) != 0)
		return (ft_putstr_fd("minishell: ", 1), ft_putstr_fd(mini->infile, 1),
			ft_putendl_fd(": no such file or directory", 1), 1);
	return (0);
}

int	recursiva(t_mini **mini)
{
	int		x;
	char	*temp;

	(*mini)->infile = NULL;
	(*mini)->didcheckenv = 0;
	x = recread(mini);
	while (x == 0)
	{
		(*mini)->status = g_status;
		temp = ft_itoa((*mini)->status);
		add_envar((*mini), "?", temp, 2);
		free (temp);
		freelist(*(*mini)->quotestmps);
		*(*mini)->quotestmps = NULL;
		(*mini)->infile = NULL;
		(*mini)->didcheckenv = 0;
		x = recread(mini);
	}
	return (0);
}

char	**preppipexlim(char *buf, char **antbuf, t_mini *mini)
{
	char	**res;
	int		i;

	i = -1;
	res = ft_calloc((ft_strcount(buf, '|') + 1) + 4, sizeof(char *));
	res[0] = ft_strdup("a");
	res[1] = ft_strdup("here_doc");
	res[2] = ft_strdup(antbuf[1]);
	res[3] = ft_strdup(antbuf[0]);
	if (checkprepaths(ft_split(antbuf[0], ' '), mini))
		return (ft_printf("mini: command not found: %s\n", antbuf[0]),
			freedoublepointer(res), freedoublepointer(antbuf), NULL);
	while (++i + 1 < ft_dstrlen(antbuf))
	{
		if (antbuf[i + 2] && checkprepaths(ft_split(antbuf[i + 2], ' '), mini))
			return (ft_printf("mini: command not found: %s\n", antbuf[0]),
				freedoublepointer(res), freedoublepointer(antbuf), NULL);
		if (antbuf[i + 2])
			res[i + 4] = ft_strdup(antbuf[i + 2]);
	}
	res[(ft_strcount(buf, '|') + 1) + 3] = NULL;
	freedoublepointer(antbuf);
	return (res);
}
