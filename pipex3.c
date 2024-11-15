/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex3.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jainavas <jainavas@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/07 19:34:41 by jainavas          #+#    #+#             */
/*   Updated: 2024/11/15 17:56:01 by jainavas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	triplepointeralloc(t_pipex *vars, int argc)
{
	int	i;

	i = 0;
	vars->cmds = malloc(sizeof(char **) * (argc - 2));
	vars->paths = malloc(sizeof(char *) * (argc - 2));
	vars->numcmds = argc - 3;
	vars->fd = malloc(sizeof(int *) * vars->numcmds + 1);
	while (i < vars->numcmds)
		vars->fd[i++] = malloc(sizeof(int) * 2);
}

int	checkpaths(t_pipex *vars)
{
	int	i;

	i = -1;
	while (++i < vars->numcmds)
	{
		if (!vars->paths[i])
			return (ft_printf("zsh: command not found: %s\n",
					vars->cmds[i][0]), -1);
	}
	return (0);
}

int	normalcmdcall(t_pipex *var, char **cmd, char *path)
{
	var->actcmd++;
	if (var->actcmd == var->numcmds - 1)
		return (lastcmdcall(var, var->cmds[var->numcmds - 1],
				var->paths[var->numcmds - 1]));
	if (!cmd)
		return (printf("cmdn"), -1);
	if (pipe(var->fd[var->actcmd]) == -1)
		return (printf("pipen"), -1);
	var->pid = fork();
	if (var->pid == 0)
	{
		closeanddupinput(var->fd[var->actcmd - 1]);
		closeanddupoutput(var->fd[var->actcmd]);
		if (execve(path, cmd, var->envp) == -1)
			return (printf("execn"), -1);
	}
	else
	{
		close(var->fd[var->actcmd - 1][WRITE_FD]);
		close(var->fd[var->actcmd - 1][READ_FD]);
		close(var->fd[var->actcmd][WRITE_FD]);
		return (normalcmdcall(var, var->cmds[var->actcmd + 1],
				var->paths[var->actcmd + 1]));
	}
	return (0);
}

void	limmitator(char *lim, int fdin)
{
	char	*buf;

	buf = get_next_line(0);
	while (ft_strncmp(buf, lim, ft_strlen(lim) - 1) != 0)
	{
		write(fdin, buf, ft_strlen(buf));
		free(buf);
		buf = get_next_line(0);
	}
	free(buf);
}
