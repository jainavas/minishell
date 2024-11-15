/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minish3.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jainavas <jainavas@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/12 21:06:20 by jainavas          #+#    #+#             */
/*   Updated: 2024/11/15 17:42:16 by jainavas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini.h"

void	docd(char *path)
{
	if (access(path, F_OK) == 0)
		chdir(path);
	else
		printf("cd: no such file or directory: %s\n", path);
}

void	debuginout(char *buf2, t_mini *mini)
{
	if (ft_strchr(buf2, '<') != NULL && ft_strchr(buf2, '<')[1] != '<')
		mini->infile = ft_substr(buf2, 0, ft_strchr(buf2, '<') - buf2 - 1);
	else
		mini->infile = ft_strdup("/dev/stdin");
	if (ft_strchr(buf2, '>') != NULL)
		mini->fileout = ft_substr(buf2, ft_strchr(buf2, '>') - buf2 + 2, ft_strlen(buf2) - (ft_strchr(buf2, '>') - buf2));
	else
		mini->fileout = ft_strdup("/dev/stdout");
	mini->out = 0;
	if (ft_strchr(buf2, '>') != NULL)
		mini->out = 1;
	mini->appendout = 0;
	if (ft_strchr(buf2, '>') != NULL && ft_strchr(buf2, '>')[1] == '>')
		mini->appendout = 1;
}