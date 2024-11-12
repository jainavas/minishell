/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minish.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jainavas <jainavas@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/09 01:58:02 by jainavas          #+#    #+#             */
/*   Updated: 2024/11/12 21:36:32 by jainavas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini.h"

int	checkkill(char *buf)
{
	if (ft_strncmp(buf, "q", 2) == 0 || ft_strncmp(buf, "kill", 5) == 0)
		return (1);
	else
		return (0);
}

void	anyfdtofile(int	fd, char *filename, int out, int app)
{
	char	*buf;
	int		fdo;

	if (out == 1)
	{
		if (access(filename, F_OK) != -1)
		{
			if (app == 1)
				fdo = open(filename, O_WRONLY | O_APPEND);
			else
				fdo = open(filename, O_WRONLY);
		}
		else
			fdo = open(filename, O_RDWR | O_CREAT | O_TRUNC, 0644);
	}
	else
		fdo = 1;
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

	if (pipe(fd) == -1)
		return (printf("pipe1"), -1);
	pid = fork();
	if (pid == 0)
	{
		dup2(fdin, fd[READ_FD]);
		close(fdin);
		dup2(fd[WRITE_FD], STDOUT_FILENO);
		closeanddupinput(fd);
		if (execve(path, cmd, mini->envp) == -1)
			return (printf("execcmd1"), -1);
	}
	else
	{
		close(fdin);
		close(fd[WRITE_FD]);
		mini->out = 0;
		freedoublepointer(cmd);
		if (mini->fileout)
			mini->out = 1;
		return (free(path), anyfdtofile(fd[READ_FD], mini->fileout, mini->out, mini->appendout), 0);
	}
	return (0);
}

char **preppipex(char *buf, char *infile, char *outfile)
{
	char	**res;
	char	**tmp;
	int		i;

	i = -1;
	if (!infile)
		infile = ft_strdup("/dev/stdin");
	else
		buf = ft_substr(buf, ft_strchr(buf, '<') - buf + 2, ft_strlen(buf) - (ft_strchr(buf, '<') - buf + 2));
	if (!outfile)
		outfile = ft_strdup("/dev/stdout");
	else
		buf = ft_substr(buf, 0, ft_strlen(buf) - (ft_strlen(buf) - (ft_strchr(buf, '>') - buf - 1)));
	tmp = ft_split(buf, '|');
	res = ft_calloc((ft_strcount(buf, '|') + 1) + 4, sizeof(char *));
	res[1] = infile;
	res[0] = ft_strdup("a");
	while (tmp[++i])
		res [i + 2] = tmp[i];
	res[i + 2] = outfile;
	free(tmp);
	return (res);
}

int	main(int argc, char **argv, char **envp)
{
	t_mini	*mini;

	mini = ft_calloc(1, sizeof(t_mini));
	mini->argc = argc;
	mini->argv = argv;
	mini->envp = envp;
	mini->infile = NULL;
	mini->fileout = NULL;
	recursiva(mini);
	free(mini->fileout);
	free(mini->infile);
	free(mini);
}