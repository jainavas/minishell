/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minish.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jainavas <jainavas@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/09 01:58:02 by jainavas          #+#    #+#             */
/*   Updated: 2024/12/18 16:31:45 by jainavas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini.h"

int	g_status;

int	checkkill(char *buf)
{
	if (ft_strcmpspace(buf, "exit") == 0)
		return (1);
	else
		return (0);
}

void	anyfdtofile(int fd, char *filename, int app)
{
	char	*buf;
	int		fdo;

	fdo = 1;
	if (access(filename, F_OK) == 0)
	{
		if (app == 1)
			fdo = open(filename, O_WRONLY | O_APPEND);
		else
			fdo = open(filename, O_WRONLY);
	}
	else if (filename)
		fdo = open(filename, O_RDWR | O_CREAT | O_TRUNC, 0644);
	buf = get_next_line(fd);
	while (buf)
	{
		write(fdo, buf, ft_strlen(buf));
		free(buf);
		buf = get_next_line(fd);
	}
	if (fdo != 0 && fdo != 1 && fdo != 2)
		close(fdo);
	if (fd != -1)
		close(fd);
}

int	alonecmdcall(int fdin, char **cmd, char *path, t_mini *mini)
{
	int	fd[2];
	int	pid;
	int	pid_status;
	int	status;

	pipe(fd);
	pid = fork();
	if (pid == 0)
	{
		alonecmdcallutils(fd, fdin);
		if (ft_strcmpalnum((*mini->mfilesout)->file, "/dev/stdout") != 0)
			dup2(fd[WRITE_FD], STDOUT_FILENO);
		closeanddupinput(fd);
		execve(path, cmd, mini->envp);
	}
	else
	{
		waitpid(pid, &pid_status, 0);
		status = 0;
		if (WIFSIGNALED(pid_status))
			status = 130;
		return (close(fdin), close(fd[WRITE_FD]), free(path),
			fdtomfiles(mini, fd[READ_FD]), status);
	}
	return (0);
}

char	**preppipex(char *buf, char *infile, char **buf2, t_mini *mini)
{
	char	**res;
	int		i;

	i = -1;
	res = ft_calloc((ft_strcount(buf, '|') + 1) + 4, sizeof(char *));
	res[1] = infile;
	res[0] = ft_strdup("a");
	while (buf2[++i])
	{
		if (checkprepaths(ft_split(buf2[i], ' '), mini))
			return (ft_printf("mini: command not found: %s\n", buf2[i]),
				freedoublepointer(buf2), freedoublepointer(res), NULL);
		res [i + 2] = ft_strdup(buf2[i]);
	}
	res[i + 2] = NULL;
	return (freedoublepointer(buf2), res);
}

int	main(int argc, char **argv, char **envp)
{
	t_mini	*mini;

	g_status = 0;
	mini = ft_calloc(1, sizeof(t_mini));
	mini->argc = argc;
	mini->argv = argv;
	mini->envp = envp;
	mini->env = init_env_vars(envp);
	mini->mfilesout = ft_calloc(1, sizeof(t_fout *));
	*(mini->mfilesout) = NULL;
	mini->quotestmps = ft_calloc(1, sizeof(t_env *));
	*(mini->quotestmps) = NULL;
	set_signals();
	recursiva(&mini);
	freelist(mini->env);
	freelist(*mini->quotestmps);
	freeoutfiles(mini->mfilesout);
	free(mini->mfilesout);
	free(mini->quotestmps);
	free(mini);
}
