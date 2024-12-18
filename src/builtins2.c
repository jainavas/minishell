/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins2.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jainavas <jainavas@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/18 00:52:59 by jainavas          #+#    #+#             */
/*   Updated: 2024/12/18 16:25:46 by jainavas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/mini.h"

int	is_bad_assignment(char *buf)
{
	int	i;

	i = -1;
	while (buf[++i])
		if (buf[i] == '=' && (!buf[i - 1] || buf[i - 1] == ' ')
			&& (!buf[i + 1] || buf[i + 1] == ' '))
			return (1);
	return (0);
}

void	chdirandoldpwd(char *new, t_mini *mini)
{
	char	*tmp;

	tmp = getcwd(NULL, 0);
	entvars(&mini->env, ft_strdup("OLDPWD"), tmp);
	chdir(new);
}
