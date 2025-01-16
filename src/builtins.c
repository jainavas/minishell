/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jainavas <jainavas@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/10 12:14:17 by mpenas-z          #+#    #+#             */
/*   Updated: 2025/01/16 15:50:14 by jainavas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

int	docd(t_cmd *cmd, t_mini *mini)
{
	t_env	*tmp;
	char	*str;
	char	*path;


	path = checkenvvars(cmd->argv[1], mini);
	if (cmd->argc > 2)
		return (ft_putstr_fd("cd: too many arguments\n", 2), mini->status = 1, 1);
	if (!path)
		return (tmp = get_env_var(&mini->env, "HOME"),
				chdirandoldpwd(ft_strdup(tmp->content), mini), 0);
	else if (path[0] == '~')
	{
		tmp = get_env_var(&mini->env, "HOME");
		str = ft_strinsertdup(strdup(path + spacesindex(path + 2) + 2), "",
				tmp->content, '~');
		chdirandoldpwd(ft_strdup(str), mini);
		return (0);
	}
	else if (!ft_strcmpalnum(path, ".."))
		return (chdirandoldpwd(prevpath(path), mini), 0);
	else if (access(path, F_OK) == 0)
		return (chdirandoldpwd(ft_strdup(path), mini), 0);
	else
		return (ft_putstr_fd("cd: no such file or directory\n", 2), mini->status = 1, 1);
}

void	doecho(t_cmd *cmd, int fd)
{
	int	i;

	if (cmd->argv[1] && ft_strcmpoptions(cmd->argv[1], "-n") != 0)
	{
		i = 1;
		write(fd, cmd->argv[i], ft_strlen(cmd->argv[i]));
		while (cmd->argv[++i])
		{
			write(fd, " ", 1);
			write(fd, cmd->argv[i], ft_strlen(cmd->argv[i]));
		}
		write(fd, "\n", 1);
	}
	else if (cmd->argv[1])
	{
		i = 2;
		write(fd, cmd->argv[i], ft_strlen(cmd->argv[i]));
		while (cmd->argv[++i])
		{
			write(fd, " ", 1);
			write(fd, cmd->argv[i], ft_strlen(cmd->argv[i]));
		}
	}
	if (fd > 2 && fd != -1)
		close(fd);
}

int	doexport(t_mini *mini, t_cmd *cmd, int fd)
{
	int		argc;
	int		status;
	char	**parsed_line;

	argc = 0;
	status = 0;
	while (cmd->argv[++argc])
	{
		if (!is_valid_identifier(cmd->argv[argc]))
		{
			ft_putstr_fd("export: not a valid identifier\n", 2);
			mini->status = 1;
		}
		else
		{
			parsed_line = ft_split(cmd->argv[argc], '=');
			if (!parsed_line[1])
				add_temp_envar(mini, parsed_line[0]);
			else
				add_envar(mini, parsed_line[0], parsed_line[1], 1);
			freedoublepointer(parsed_line);
		}
	}
	if (argc == 1)
		print_temp_env(mini->env, fd);
	else if (fd > 2 && fd != -1)
		close(fd);
	return (status);
}

void	dounset(t_mini *mini, t_cmd	*cmd)
{
	int		argc;

	(void)mini;
	argc = 0;
	if (cmd->argc == 1)
		return ;
	else if (cmd->argc >= 2)
	{
		argc = 0;
		while (cmd->argv[++argc])
			remove_envar(mini, cmd->argv[argc]);
	}
}

int	builtins(t_mini *mini, t_cmd *cmd)
{
	if (cmd->prev)
		close(cmd->prev->fd[READ_FD]);
	cmd->isbltin = 1;
	if (checkkill(cmd->cmd))
	{
		if (cmd->argc > 2)
			return (ft_putstr_fd("exit: too many arguments\n", 2), exit(1), 1);
		if (cmd->argv[1] && checkovrfandchar(cmd->argv[1]))
			return (exit(ft_atoi(cmd->argv[1])), 0);
		if (cmd->argv[1])
			return (ft_putstr_fd("exit: incorrect arguments\n", 2), exit(2), 1);
		else
			return (exit(0), 0);
	}
	pipe(cmd->fd);
	if (ft_strcmpspace("cd", cmd->cmd) == 0)
		return (docd(cmd, mini), close(cmd->fd[READ_FD]), close(cmd->fd[WRITE_FD]), 0);
	if (ft_strcmpspace("unset", cmd->cmd) == 0)
		return (dounset(mini, cmd), close(cmd->fd[READ_FD]), close(cmd->fd[WRITE_FD]), 0);
	if (ft_strchr(cmd->cmd, '=') && cmd->argc == 1
		&& ft_isgroup(ft_strchr(cmd->cmd, '=') + 1, ft_isbashprotected) == 0)
	{
		cmd->cmd = checkenvvars(cmd->cmd, mini);
		entvars(&mini->env, ft_strndup(cmd->cmd, ft_strchr(cmd->cmd, '=') - cmd->cmd),
			ft_strdup(ft_strchr(cmd->cmd, '=') + 1));
		return (close(cmd->fd[READ_FD]), close(cmd->fd[WRITE_FD]), 0);
	}
	if (ft_strcmpspace("export", cmd->cmd) == 0)
		return (doexport(mini, cmd, cmd->fd[WRITE_FD]), cmd->fd[READ_FD]);
	if (ft_strcmpspace("env", cmd->cmd) == 0)
		return (print_envfd(mini->env, cmd->fd[WRITE_FD]), cmd->fd[READ_FD]);
	if (ft_strcmpspace("echo", cmd->cmd) == 0)
		return (doecho(cmd, cmd->fd[WRITE_FD]), cmd->fd[READ_FD]);
	return (close(cmd->fd[WRITE_FD]), close(cmd->fd[READ_FD]), 0);
}

int	isbuiltin(t_cmd *cmd)
{
	if (ft_strcmpspace(cmd->cmd, "exit") == 0)
		return (1);
	if (ft_strcmpspace("cd", cmd->cmd) == 0)
		return (1);
	if (ft_strcmpspace("unset", cmd->cmd) == 0)
		return (1);
	if (ft_strchr(cmd->cmd, '=') && cmd->argc == 1
			&& ft_isgroup(ft_strchr(cmd->cmd, '=') + 1, ft_isbashprotected) == 0)
		return (1);
	if (ft_strcmpspace("export", cmd->cmd) == 0)
		return (1);
	if (ft_strcmpspace("env", cmd->cmd) == 0)
		return (1);
	if (ft_strcmpspace("echo", cmd->cmd) == 0)
		return (1);
	return (0);
}
