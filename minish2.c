/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minish2.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jainavas <jainavas@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/10 15:56:12 by jainavas          #+#    #+#             */
/*   Updated: 2024/11/19 18:46:51 by jainavas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini.h"

int	recread(t_mini *mini)
{
	char	**buf;
	char	*buf2;
	int		fdin;
	int		pid;

	fdin = 0;
	buf2 = readline("minishell% ");
	add_history(buf2);
	buf = NULL;
	if (checkkill(buf2))
		return (free(buf2), rl_clear_history(), 1);
	if (ft_strncmp("cd ", buf2, 3) == 0)
		return (docd(&buf2[3]), free(buf2), 0);
	buf = ft_split_cmds(buf2);
	debuginout(buf2, buf, mini);
	if ((ft_strchr(buf2, '<') != NULL && ft_strchr(buf2, '<')[1] == '<') && ft_strchr(buf2, '|') != NULL)
	{
		buf = preppipexlim(buf2, buf);
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
		buf = preppipex(buf2, mini->infile, mini->fileout, buf);
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
		if (ft_strcount(buf2, '<') == 2)
			return (free(buf2), dolimitonecmd(buf, mini));
		buf2 = debugbuffer(buf2);
		free(buf2);
		buf2 = pathseek(buf, mini->envp);
		if (mini->infile)
			fdin = open(mini->infile, O_RDONLY);
		if (!buf2)
			return (write(1, "Unknown command\n", 16), free(mini->infile), free(mini->fileout), freedoublepointer(buf), free(buf2), 0);
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

char **preppipexlim(char *buf, char **antbuf)
{
	char	**res;
	int		i;

	i = -1;
	res = ft_calloc((ft_strcount(buf, '|') + 1) + 5, sizeof(char *));
	res[0] = ft_strdup("a");
	res[1] = ft_strdup("here_doc");
	res[2] = antbuf[1];
	res[3] = antbuf[0];
	if (ft_strcount(buf, '|') == 1)
		res[4] = antbuf[2];
	else
	{
		while (++i + 2 < ft_dstrlen(antbuf))
			res[i + 4] = antbuf[i + 2];
	}
	if (ft_strchr(buf, '>') == NULL)
	 	res[(ft_strcount(buf, '|') + 1) + 3] = ft_strdup("/dev/stdout");
	else
		res[(ft_strcount(buf, '|') + 1) + 3] = antbuf[ft_dstrlen(antbuf) - 1];
	res[(ft_strcount(buf, '|') + 1) + 4] = NULL;
	free(antbuf);
	return (res);
}
