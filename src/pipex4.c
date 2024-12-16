/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex4.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jainavas <jainavas@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/10 19:34:43 by jainavas          #+#    #+#             */
/*   Updated: 2024/12/16 18:29:07 by jainavas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

int	limvardefs(t_pipex *vars, char **argv, int argc)
{
	int		i;

	i = -1;
	triplepointeralloc(vars, argc - 2);
	while (argv[++i + 3])
		vars->cmds[i] = ft_split(argv[i + 3], ' ');
	vars->cmds[i] = NULL;
	i = -1;
	while (++i < argc - 4)
	{
		vars->paths[i] = pathseek(vars->cmds[i], vars->envp);
		if (!vars->paths[i])
			break ;
	}
	vars->paths[i] = NULL;
	if (checkpaths(vars) == -1)
		return (-1);
	vars->fdin = open("tmp_heredoc", O_CREAT | O_WRONLY, 0777);
	vars->pwd = pwdseek(vars);
	limmitator(argv[2], vars->fdin);
	close(vars->fdin);
	vars->filein = ft_strdup("/tmp_heredoc");
	vars->fdin = open("tmp_heredoc", O_RDONLY);
	return (firstcmdcall(vars, vars->cmds[0], vars->paths[0]));
}

char	*pwdseek(t_pipex *var)
{
	int			fd[2];
	int			pid;
	char		*tmp;
	char *const	argv[] = {"pwd", NULL};

	if (pipe(fd) == -1)
		return (NULL);
	pid = fork();
	if (pid == 0)
	{
		closeanddupoutput(fd);
		if (execve("/usr/bin/pwd", argv, var->envp) == -1)
			return (NULL);
	}
	else
	{
		close(fd[WRITE_FD]);
		wait(NULL);
		tmp = get_next_line(fd[READ_FD]);
		tmp[ft_strlen(tmp) - 1] = '\0';
		return (close(fd[READ_FD]), tmp);
	}
	return (NULL);
}

int	limornot(int argc, char **argv, t_pipex *vars)
{
	if (ft_strncmp(argv[1], "here_doc", 8) == 0)
	{
		vars->lim = ft_strdup(argv[2]);
		if (limvardefs(vars, argv, argc) != 0)
			return (freepipex(vars), 1);
	}
	else
	{
		if (vardefs(vars, argv, argc) != 0)
			return (freepipex(vars), 1);
		if (checks(argv, vars) != 0)
			return (freepipex(vars), 1);
		if (firstcmdcall(vars, vars->cmds[0], vars->paths[0]) != 0)
			return (freepipex(vars), 1);
	}
	return (0);
}
