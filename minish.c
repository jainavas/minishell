/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minish.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jainavas <jainavas@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/09 01:58:02 by jainavas          #+#    #+#             */
/*   Updated: 2024/11/09 02:42:57 by jainavas         ###   ########.fr       */
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
	buf = ft_split(buf2, ' ');
	alonecmdcall(0, buf, pathseek(buf, envp), mini);
}