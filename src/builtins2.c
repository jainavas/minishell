/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins2.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jainavas <jainavas@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/18 00:52:59 by jainavas          #+#    #+#             */
/*   Updated: 2024/12/24 16:39:41 by mpenas-z         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/mini.h"

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
}
