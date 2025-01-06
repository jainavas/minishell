/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minish.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jainavas <jainavas@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/09 01:58:02 by jainavas          #+#    #+#             */
/*   Updated: 2025/01/06 03:17:39 by jainavas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini.h"

int	g_status;

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
	// if (fd != -1)
	// 	close(fd);
}

int	alonecmdcall(int fdin, char **cmd, char *path, t_mini *mini)
{
	int	fd[2];
	int	pid;
	int	pid_status;

	pipe(fd);
	pid = fork();
	if (pid == 0)
	{
		alonecmdcallutils(fd, fdin);
		if (cmdcount(&mini->header) != 1 || *((t_cmd *)mini->header->content)->outfiles != NULL)
			dup2(fd[WRITE_FD], STDOUT_FILENO);
		closeanddupinput(fd);
		execve(path, cmd, envtodoublechar(mini->env));
	}
	else
	{
		waitpid(pid, &pid_status, 0);
		if (g_status == 130)
			write(1, "\n", 1);
		if (fdin > 2)
			close(fdin);
		return (close(fd[WRITE_FD]), free(path), fd[READ_FD]);
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
			return (ft_putstr_fd("mini: command not found\n", 2),
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
	mini->status = 0;
	mini->envp = envp;
	mini->env = init_env_vars(envp);
	add_envar(mini, "?", "0", 2);
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
	return (g_status);
}
