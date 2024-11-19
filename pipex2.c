/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jainavas <jainavas@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/16 20:20:50 by jainavas          #+#    #+#             */
/*   Updated: 2024/11/19 16:10:40 by jainavas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	freepipex(t_pipex *vars)
{
	int		i;
	char	*buf;

	i = -1;
	buf = NULL;
	close(vars->fdin);
	if (vars->pwd)
	{
		buf = ft_strjoin_gnl(vars->pwd, vars->filein);
		unlink(buf);
		free(buf);
	}
	while (++i < vars->numcmds)
		freedoublepointer(vars->cmds[i]);
	free(vars->cmds);
	if (vars->lim)
		free(vars->lim);
	free(vars->filein);
	freedoublepointer(vars->paths);
	freedp(vars->fd);
	free(vars);
}

char	*pathseek(char **args, char **envp)
{
	int			fd[2];
	int			pid;
	char		*tmp;
	char *const	argv[] = {"which", args[0], NULL};

	if (pipe(fd) == -1)
		return (NULL);
	pid = fork();
	if (pid == 0)
	{
		closeanddupoutput(fd);
		if (execve("/usr/bin/which", argv, envp) == -1)
			return (NULL);
	}
	else
	{
		close(fd[WRITE_FD]);
		wait(NULL);
		tmp = get_next_line(fd[READ_FD]);
		if (!tmp)
			return (NULL);
		tmp[ft_strlen(tmp) - 1] = '\0';
		return (close(fd[READ_FD]), tmp);
	}
	return (NULL);
}

int	checks(char **argv, t_pipex *var)
{
	int	i;

	i = -1;
	while (++i < var->numcmds)
		if (!var->paths[i] || access(var->paths[i], X_OK) != 0)
			return (-1);
	if (access(argv[1], R_OK) != 0)
		return (-1);
	return (0);
}

void	fdtofile(t_pipex *var, char *filename)
{
	if (var->lim && access(filename, F_OK) != -1)
		var->fdout = open(filename, O_WRONLY | O_APPEND);
	else
		var->fdout = open(filename, O_RDWR | O_CREAT | O_TRUNC, 0644);
	var->buf = get_next_line(var->fd[var->numcmds - 1][READ_FD]);
	while (var->buf)
	{
		write(var->fdout, var->buf, ft_strlen(var->buf));
		free(var->buf);
		var->buf = get_next_line(var->fd[var->numcmds - 1][READ_FD]);
	}
	close(var->fdout);
	close(var->fd[var->numcmds - 1][READ_FD]);
}

void	closeanddupoutput(int fd[2])
{
	close(fd[READ_FD]);
	dup2(fd[WRITE_FD], STDOUT_FILENO);
	close(fd[WRITE_FD]);
}
