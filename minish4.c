/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minish4.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jainavas <jainavas@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/19 20:05:57 by jainavas          #+#    #+#             */
/*   Updated: 2024/11/21 12:37:51 by jainavas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini.h"

int	dolimwithpipe(char *buf2, char **buf, t_mini *mini)
{
	int	pid;

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
	return (0);
}

int	dopipes(char *buf2, char **buf, t_mini *mini)
{
	int	pid;
	int	status;

	buf = preppipex(buf2, mini->infile, mini->fileout, buf);
	pid = fork();
	if (pid == 0)
	{
		pipex(((ft_strcount(buf2, '|') + 1) + 3), buf, mini->envp, mini);
		return (free(buf2), freedoublepointer(buf), 1);
	}
	else
	{
		wait(&status);
		return (free(buf2), freedoublepointer(buf), 0);
	}
}

int	docmd(char *buf2, char **buf, t_mini *mini)
{
	int	fdin;
	char **aux;

	fdin = 0;
	if (ft_strcount(buf2, '<') == 2)
		return (free(buf2), dolimitonecmd(buf, mini));
	free(buf2);
	buf2 = ft_strdup(buf[0]);
	free(buf2);
	buf2 = pathseek(buf, mini->envp);
	if (mini->infile)
		fdin = open(mini->infile, O_RDONLY);
	if (!buf2)
		return (write(1, "Unknown command\n", 16), free(mini->infile),
			free(mini->fileout), freedoublepointer(buf), free(buf2), 0);
	aux = ft_split(buf[0], ' ');
	alonecmdcall(fdin, aux, pathseek(aux, mini->envp), mini);
	free(mini->fileout);
	free(mini->infile);
	return (free(buf2), freedoublepointer(aux), freedoublepointer(buf), 0);
}
