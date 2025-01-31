/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minish6.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jainavas <jainavas@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/28 17:07:05 by jainavas          #+#    #+#             */
/*   Updated: 2025/01/31 12:26:15 by mpzamora         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./minishell.h"

char	*pathseekenv(char **args, char **envp)
{
	int			fd[2];
	int			pid;
	int			status;
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
		waitpid(pid, &status, 0);
		tmp = get_next_line(fd[READ_FD]);
		if (!tmp)
			return (close(fd[READ_FD]), NULL);
		return (close(fd[READ_FD]), tmp[ft_strlen(tmp) - 1] = '\0', tmp);
	}
	return (NULL);
}

int	counttmps(t_env *lst)
{
	t_env	*new;
	int		i;

	new = lst;
	i = 0;
	while (new != NULL)
	{
		if (strncmp("holatmp_", new->name, 8) == 0)
			i++;
		new = new->next;
	}
	return (i);
}

void	putoutfn(t_fout **head)
{
	t_fout	*tmp;
	int		i;

	tmp = *head;
	i = 0;
	while (tmp)
	{
		tmp->foutn = ++i;
		tmp = tmp->next;
	}
}

int	cmd_in_path(char *cmd_path, char *path, char **envp)
{
	char	**path_dirs;
	char	*aux_cmd;
	int		i;

	cmd_path = pathseekenv(&cmd_path, envp);
	if (!cmd_path)
		return (freedoublepointer(envp), 1);
	aux_cmd = ft_strrchr(cmd_path, '/');
	*aux_cmd = '\0';
	path_dirs = ft_split(path, ':');
	if (!path_dirs)
		return (free(cmd_path), freedoublepointer(envp), 1);
	i = -1;
	while (path_dirs[++i])
		if (!ft_strncmp(path_dirs[i], cmd_path, ft_strlen(path_dirs[i]))
			&& !ft_strncmp(path_dirs[i], cmd_path, ft_strlen(cmd_path)))
			return (free(cmd_path), freedoublepointer(path_dirs),
				freedoublepointer(envp), 1);
	return (free (cmd_path), freedoublepointer(path_dirs),
		freedoublepointer(envp), 0);
}
