/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minish4.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jainavas <jainavas@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/19 20:05:57 by jainavas          #+#    #+#             */
/*   Updated: 2025/01/30 20:29:49 by mpzamora         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	closeanddupinput(int fd[2])
{
	close(fd[WRITE_FD]);
	dup2(fd[READ_FD], STDIN_FILENO);
	close(fd[READ_FD]);
}

void	closeanddupoutput(int fd[2])
{
	close(fd[READ_FD]);
	dup2(fd[WRITE_FD], STDOUT_FILENO);
	close(fd[WRITE_FD]);
}

void	limmitator(char *lim, int fdin, struct mini *mini)
{
	char	*buf;

	buf = readline("> ");
	if (!buf)
		return ;
	while (ft_strcmpspace(buf, lim) != 0)
	{
		buf = checkenvvars(buf, mini);
		write(fdin, buf, ft_strlen(buf));
		write(fdin, "\n", 1);
		free(buf);
		buf = readline("> ");
		if (!buf)
			return ;
	}
	free(buf);
}

void	freelist(t_env *lst)
{
	t_env	*new;

	while (lst != NULL)
	{
		new = lst;
		lst = new->next;
		free(new->content);
		free(new->name);
		free(new);
	}
}

int	path_exists(t_mini *mini, t_cmd *cmd)
{
	t_env	*env;

	env = mini->env;
	while (env)
	{
		if (!ft_strncmp("PATH", env->name, 4))
			if (cmd_in_path(cmd->cmd, env->content, envtodoublechar(mini->env)))
				return (1);
		env = env->next;
	}
	return (0);
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
