/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex3.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jainavas <jainavas@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/07 19:34:41 by jainavas          #+#    #+#             */
/*   Updated: 2024/12/06 23:37:10 by mpenas-z         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"
#include "mini.h"
#include "readline/readline.h"

void	triplepointeralloc(t_pipex *vars, int argc)
{
	int	i;

	i = 0;
	vars->cmds = malloc(sizeof(char **) * (argc - 1));
	vars->paths = malloc(sizeof(char *) * (argc - 1));
	vars->numcmds = argc - 2;
	vars->fd = ft_calloc(sizeof(int *), vars->numcmds + 1);
	while (i < vars->numcmds)
		vars->fd[i++] = malloc(sizeof(int) * 2);
}

int	checkpaths(t_pipex *vars)
{
	int	i;

	i = -1;
	while (++i < vars->numcmds - 1)
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
		signal(SIGINT, SIG_DFL);
		signal(SIGQUIT, SIG_DFL);
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

	buf = readline("> ");
	while (ft_strncmp(buf, lim, ft_strlen(lim)) != 0)
	{
		write(fdin, buf, ft_strlen(buf));
		write(fdin, "\n", 1);
		free(buf);
		buf = readline("> ");
	}
	free(buf);
}
