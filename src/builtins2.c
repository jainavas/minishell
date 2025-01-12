/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins2.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jainavas <jainavas@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/18 00:52:59 by jainavas          #+#    #+#             */
/*   Updated: 2025/01/12 22:33:32 by jainavas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

int	is_valid_identifier(char *buf)
{
	int		i;

	i = -1;
	while (buf[++i])
		if (buf[i] == '=' 
			&& (!buf[i - 1] || buf[i - 1] == ' ' || buf[i - 1] == '=')
			&& (!buf[i + 1] || buf[i + 1] == ' ' || buf[i + 1] == '='))
			return (0);
	i = -1;
	while (buf[++i] && buf[i] != '=')
		if ((i == 0 && (buf[i] >= '0' && buf[i] <= '9')) || buf[i] == '-')
			return (0);
	return (1);
}

int	are_numbers(char *buf)
{
	int	i;
	
	i = -1;
	while (buf[++i])
		if ((buf[i] < '0' || buf[i] > '9') && buf[i] != '+' && buf[i] != '-')
			return (0);
	return (1);
}

int	checkkill(char *buf)
{
	char	**parsed_line;
	int		exit_status;

	if (ft_strcmpspace(buf, "exit") == 0)
	{
		if (!ft_strchr(buf, ' '))
			return (g_status = 1, -1);
		else
		{
			parsed_line = ft_split(buf, ' ');
			if (!are_numbers(parsed_line[1]))
				return (ft_putstr_fd("exit: numeric argument required\n", 2) \
					, 2);
			exit_status = ft_atoi(parsed_line[1]);
			while (exit_status > 256)
				exit_status = exit_status - 256;
			g_status = exit_status;
			return (-1);
		}
	}
	else
		return (0);
}

void	chdirandoldpwd(char *new, t_mini *mini)
{
	char	*tmp;

	tmp = getcwd(NULL, 0);
	entvars(&mini->env, ft_strdup("OLDPWD"), tmp);
	chdir(new);
	entvars(&mini->env, ft_strdup("PWD"), getcwd(NULL, 0));
	free(new);
}

int	checkpermission(char *file, int rwx, t_mini *mini)
{
	if (access(file, F_OK) == -1)
		return (ft_putstr_fd("File not found\n", 2), mini->status = 1, 0);
	if (rwx == 1)
	{
		if (access(file, R_OK) == -1)
		{
			if (cmdcount(mini->header) <= 1)
				return (ft_putstr_fd("Access denied\n", 2), mini->status = 1, -1);
			else
				return (mini->status = 126, -1);
		}
	}
	if (rwx == 2)
	{
		if (access(file, W_OK) == -1)
		{
			if (cmdcount(mini->header) <= 1)
				return (ft_putstr_fd("Access denied\n", 2), mini->status = 1, -1);
			else
				return (mini->status = 126, -1);
		}
	}
	if (rwx == 3)
	{
		if (access(file, X_OK) == -1)
		{
			if (cmdcount(mini->header) <= 1)
				return (ft_putstr_fd("Access denied\n", 2), mini->status = 1, -1);
			else
				return (mini->status = 126, -1);
		}
	}
	return (1);
}

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

char	*prevcwd()
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
