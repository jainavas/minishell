/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minish.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jainavas <jainavas@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/09 01:58:02 by jainavas          #+#    #+#             */
/*   Updated: 2024/11/10 02:37:40 by jainavas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini.h"

void	anyfdtofile(int	fd, char *filename, int out)
{
	char	*buf;
	int		fdo;

	if (out == 1)
		fdo = open(filename, O_RDWR | O_CREAT | O_TRUNC, 0644);
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
		return (anyfdtofile(fd[READ_FD], NULL, 0), 0);
	}
	return (0);
}

char **preppipex(char *buf, char *infile, char *outfile)
{
	//ls -la | wc -l
	char	**res;
	char	**tmp;
	int		i;

	i = -1;
	if (!infile)
		infile = ft_strdup("/dev/stdin");
	if (!outfile)
		outfile = ft_strdup("/dev/stdout");
	tmp = ft_split(buf, '|');
	res = malloc(sizeof(char *) * dstrlen(tmp) + 3);
	res[1] = infile;
	res[0] = ft_strdup("a");
	while (tmp[++i])
		res [i + 2] = tmp[i];
	res[i + 2] = outfile;
	return (res);
}

int	main(int argc, char **argv, char **envp)
{
	char	**buf;
	char	*buf2;
	t_mini	*mini;

	mini = ft_calloc(1, sizeof(t_mini));
	mini->argc = argc;
	mini->argv = argv;
	mini->envp = envp;
	buf2 = get_next_line(0);
	buf2[ft_strlen(buf2) - 1] = '\0';
	if (ft_strchr(buf2, '|') != NULL)
	{
		buf = preppipex(buf2, NULL, NULL);
		printf("%d", dstrlen(buf));
		pipex(dstrlen(buf), buf, envp);
	}
	else
	{
		buf = ft_split(buf2, ' ');
		alonecmdcall(0, buf, pathseek(buf, envp), mini);
	}
}