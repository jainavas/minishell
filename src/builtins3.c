/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins3.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jainavas <jainavas@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/20 18:39:14 by jainavas          #+#    #+#             */
/*   Updated: 2025/01/20 19:54:09 by jainavas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

char	*prevpath(char *path)
{
	char	*tmp;
	char	*tmp1;
	char	*tmp2;

	if (!path)
		return (prevcwd());
	tmp = ft_strtrim(path, "./");
	tmp1 = prevcwd();
	tmp2 = pathbuilder(tmp1, tmp);
	return (free(tmp), free(tmp1), tmp2);
}

char	*prevcwd(void)
{
	char	*tmp;
	char	*tmp2;

	tmp = getcwd(NULL, 0);
	if (!ft_strncmp("/home", tmp, ft_strlen(tmp)))
		return (free(tmp), ft_strdup("/"));
	tmp2 = ft_strndup(tmp, (ft_strrchr(tmp, '/') - tmp));
	return (free(tmp), tmp2);
}

int	checkpermouts(t_cmd *cmd, char *file, t_mini *mini)
{
	if (!cmd)
		return (-1);
	if (access(file, F_OK) == -1)
		return (ft_putstr_fd("File not found\n", 2), mini->status = 1, 0);
	if (access(file, W_OK) == -1)
	{
		if (cmdcount(mini->header) == cmd->cmdn)
			return (ft_putstr_fd("Access denied\n", 2), mini->status = 1, -1);
		else
			return (mini->status = 126, -1);
	}
	return (1);
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

int	tildecasecd(t_env *tmp, t_mini *mini, char *path)
{
	char	*str;

	tmp = get_env_var(&mini->env, "HOME");
	if (!tmp)
		return (1);
	str = ft_strinsertdup(strdup(path + spacesindex(path + 2) + 2), "",
			tmp->content, '~');
	chdirandoldpwd(ft_strdup(str), mini);
	return (0);
}
