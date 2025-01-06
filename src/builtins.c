/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jainavas <jainavas@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/10 12:14:17 by mpenas-z          #+#    #+#             */
/*   Updated: 2025/01/06 02:56:55 by jainavas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/mini.h"

int	docd(char *path, t_mini *mini)
{
	t_env	*tmp;
	char	*str;

	if (!path)
	{
		tmp = get_env_var(&mini->env, "HOME");
		chdirandoldpwd(tmp->content, mini);
		return (0);
	}
	else if (path[0] == '~')
	{
		tmp = get_env_var(&mini->env, "HOME");
		str = ft_strinsertdup(strdup(path + spacesindex(path + 2) + 2), "",
				tmp->content, '~');
		chdirandoldpwd(str, mini);
		return (free(str), 0);
	}
	else if (access(path, F_OK) == 0)
		return (chdirandoldpwd(path, mini), 0);
	else
		return (ft_putstr_fd("cd: no such file or directory\n", 2), 1);
}

void	doecho(t_cmd *cmd, int fd)
{
	int	i;

	if (ft_strcmpoptions(cmd->argv[1], "-n") != 0)
	{
		i = 0;
		while (cmd->argv[++i])
			write(fd, cmd->argv[i], ft_strlen(cmd->argv[i]));
		write(fd, "\n", 1);
	}
	else
	{
		i = 1;
		while (cmd->argv[++i])
			write(fd, cmd->argv[i], ft_strlen(cmd->argv[i]));
	}
	if (fd > 2)
		close(fd);
}
//export WIP, without options works fine, need to fix option handle got frozen
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
			status = 1;
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
	else
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
	int	fd[2];

	if (ft_strcmpspace("cd", cmd->cmd) == 0)
		return (docd(checkenvvars(cmd->argv[1], mini), mini), 0);
	if (ft_strcmpspace("unset", cmd->cmd) == 0)
		return (dounset(mini, cmd), 0);
	// WIP with new parse
	// if (ft_strchr(buf2, '=') && ft_strchr(buf2, '=')[-1] != ' '
	// 	&& ft_strchr(buf2, '=')[1] != ' '
	// 	&& ft_isgroup(ft_strchr(buf2, '=') + 1, ft_isbashprotected) == 0)
	// {
	// 	buf2 = checkenvvars(buf2, mini);
	// 	entvars(&mini->env, ft_substr(buf2, 0,
	// 			ft_strchr(buf2, '=') - buf2),
	// 		ft_strdup(ft_strchr(buf2, '=') + 1));
	// 	return (free(buf2), -1);
	// }
	pipe(fd);
	if (ft_strcmpspace("export", cmd->cmd) == 0)
		return (doexport(mini, cmd, fd[WRITE_FD]), fd[READ_FD]);
	if (ft_strcmpspace("env", cmd->cmd) == 0)
		return (print_envfd(mini->env, fd[WRITE_FD]), fd[READ_FD]);
	if (ft_strcmpspace("echo", cmd->cmd) == 0)
		return (doecho(cmd, fd[WRITE_FD]), fd[READ_FD]);
	return (close(fd[READ_FD]), close(fd[WRITE_FD]), -2);
}
