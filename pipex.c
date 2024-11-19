/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jainavas <jainavas@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/14 16:54:34 by jainavas          #+#    #+#             */
/*   Updated: 2024/11/19 20:22:50 by jainavas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"
#include "mini.h"

void	closeanddupinput(int fd[2])
{
	close(fd[WRITE_FD]);
	dup2(fd[READ_FD], STDIN_FILENO);
	close(fd[READ_FD]);
}

int	lastcmdcall(t_pipex *var, char **cmd, char *path)
{
	if (!cmd)
		return (printf("cmdlast"), -1);
	if (pipe(var->fd[var->actcmd]) == -1)
		return (printf("pipelast"), -1);
	var->pid = fork();
	if (var->pid == 0)
	{
		closeanddupinput(var->fd[var->actcmd - 1]);
		closeanddupoutput(var->fd[var->actcmd]);
		if (execve(path, cmd, var->envp) == -1)
			return (printf("execcmdlast"), -1);
	}
	else
	{
		close(var->fd[var->actcmd - 1][WRITE_FD]);
		close(var->fd[var->actcmd - 1][READ_FD]);
		close(var->fd[var->actcmd][WRITE_FD]);
		close(var->fdout);
		return (anyfdtofile(var->fd[var->numcmds - 1][READ_FD],
			var->output, var->out, var->app), 0);
	}
	return (0);
}

int	firstcmdcall(t_pipex *var, char **cmd, char *path)
{
	var->actcmd = 0;
	if (!cmd)
		return (printf("cmd1"), freepipex(var), -1);
	if (pipe(var->fd[var->actcmd]) == -1)
		return (printf("pipe1"), -1);
	var->pid = fork();
	if (var->pid == 0)
	{
		dup2(var->fdin, var->fd[var->actcmd][READ_FD]);
		close(var->fdin);
		dup2(var->fd[var->actcmd][WRITE_FD], STDOUT_FILENO);
		closeanddupinput(var->fd[var->actcmd]);
		if (execve(path, cmd, var->envp) == -1)
			return (printf("execcmd1"), -1);
	}
	else
	{
		close(var->fdin);
		return (normalcmdcall(var, var->cmds[var->actcmd + 1],
				var->paths[var->actcmd + 1]));
	}
	return (0);
}

int	vardefs(t_pipex *vars, char **argv, int argc)
{
	int	i;

	i = -1;
	triplepointeralloc(vars, argc);
	while (argv[++i + 3])
		vars->cmds[i] = ft_split(argv[i + 2], ' ');
	vars->cmds[i] = NULL;
	vars->output = argv[argc - 1];
	i = -1;
	while (++i < argc - 3)
		vars->paths[i] = pathseek(vars->cmds[i], vars->envp);
	vars->paths[i] = NULL;
	if (checkpaths(vars) == -1)
		return (-1);
	vars->fdin = open(argv[1], O_RDONLY);
	if (vars->fdin == -1)
		return (ft_printf("zsh: no such file or directory: %s\n", argv[1]), -1);
	return (0);
}

int	pipex(int argc, char **argv, char **envp, t_mini *mini)
{
	t_pipex	*vars;
	int		i;
	int		s;

	i = -1;
	s = 0;
	vars = ft_calloc(1, sizeof(t_pipex));
	vars->envp = envp;
	vars->out = mini->out;
	vars->app = mini->appendout;
	if (argc < 5)
		return (free(vars), 2);
	s = limornot(argc, argv, vars);
	if (s != 0)
		return (s);
	close(STDOUT_FILENO);
	close(STDERR_FILENO);
	while (++i < vars->numcmds)
		wait(NULL);
	freepipex(vars);
	return (0);
}
