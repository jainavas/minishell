/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minish.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jainavas <jainavas@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/09 01:58:02 by jainavas          #+#    #+#             */
/*   Updated: 2024/11/21 18:56:31 by jainavas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini.h"

int	checkkill(char *buf)
{
	if (ft_strncmp(buf, "exit", 5) == 0 || ft_strncmp(buf, "kill", 5) == 0)
		return (1);
	else
		return (0);
}

void	anyfdtofile(int fd, char *filename, int out, int app)
{
	char	*buf;
	int		fdo;

	fdo = 1;
	if (out == 1)
	{
		if (access(filename, F_OK) == 0)
		{
			if (app == 1)
				fdo = open(filename, O_WRONLY | O_APPEND);
			else
				fdo = open(filename, O_WRONLY);
		}
		else if (filename)
			fdo = open(filename, O_RDWR | O_CREAT | O_TRUNC, 0644);
	}
	buf = get_next_line(fd);
	while (buf)
	{
		write(fdo, buf, ft_strlen(buf));
		free(buf);
		buf = get_next_line(fd);
	}
	close(fdo);
	close(fd);
}

int	alonecmdcall(int fdin, char **cmd, char *path, t_mini *mini)
{
	int	fd[2];
	int	pid;

	pipe(fd);
	pid = fork();
	if (pid == 0)
	{
		dup2(fdin, fd[READ_FD]);
		close(fdin);
		dup2(fd[WRITE_FD], STDOUT_FILENO);
		closeanddupinput(fd);
		execve(path, cmd, mini->envp);
	}
	else
	{
		close(fdin);
		close(fd[WRITE_FD]);
		mini->out = 0;
		if (mini->fileout)
			mini->out = 1;
		return (free(path), anyfdtofile(fd[READ_FD], mini->fileout,
				mini->out, mini->appendout), 0);
	}
	return (0);
}

char	**preppipex(char *buf, char *infile, char *outfile, char **buf2)
{
	char	**res;
	int		i;

	i = -1;
	res = ft_calloc((ft_strcount(buf, '|') + 1) + 4, sizeof(char *));
	res[1] = infile;
	if (ft_strncmp(res[1], "/dev/stdin", 11) != 0)
		i++;
	res[0] = ft_strdup("a");
	if (ft_strncmp(outfile, "/dev/stdout", 12) == 0)
	{
		while (buf2[++i])
			res [i + 2] = buf2[i];
		res[i + 2] = outfile;
		res[i + 3] = NULL;
	}
	else
	{
		while (buf2[++i])
			res [i + 2] = buf2[i];
		res[i + 2] = NULL;
	}
	free(buf2);
	return (res);
}

int	main(int argc, char **argv, char **envp)
{
	t_mini	*mini;

	mini = ft_calloc(1, sizeof(t_mini));
	mini->argc = argc;
	mini->argv = argv;
	mini->envp = envp;
	recursiva(mini);
	free(mini);
}
