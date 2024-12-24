/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins2.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jainavas <jainavas@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/18 00:52:59 by jainavas          #+#    #+#             */
/*   Updated: 2024/12/24 15:38:45 by mpenas-z         ###   ########.fr       */
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

void	chdirandoldpwd(char *new, t_mini *mini)
{
	char	*tmp;

	tmp = getcwd(NULL, 0);
	entvars(&mini->env, ft_strdup("OLDPWD"), tmp);
	chdir(new);
}
