/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fileshandle3.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jainavas <jainavas@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/20 21:14:16 by jainavas          #+#    #+#             */
/*   Updated: 2025/01/20 22:30:12 by jainavas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

char	*fileseekutils(t_ffdr *var)
{
	t_ffdr	*tmp;
	char	*ret;

	if (var->entry->d_name[0] == '.')
		return (NULL);
	var->pathf = pathbuilder(var->directory, var->entry->d_name);
	if (stat(var->pathf, &var->statbuf) == -1)
		return (free(var->pathf), NULL);
	if (S_ISDIR(var->statbuf.st_mode))
	{
		tmp = ft_calloc(1, sizeof(t_ffdr));
		tmp->f = ft_strdup(var->f);
		tmp->directory = var->pathf;
		var->pathf = fileseek(tmp);
		if (var->pathf)
			return (ret = ft_strdup(var->pathf), freeffdr(var), ret);
	}
	else
	{
		if (!ft_strcmpff(pathbuilder(var->directory, var->f),
				pathbuilder(var->directory, var->entry->d_name)))
			return (ret = ft_strdup(var->pathf), freeffdr(var), ret);
		return (free(var->pathf), var->pathf = NULL, NULL);
	}
	return (NULL);
}

char	*dirseekutils(t_ffdr *var)
{
	char	*ret;

	if (var->entry->d_name[0] == '.')
		return (NULL);
	var->pathf = pathbuilder(var->directory, var->entry->d_name);
	if (stat(var->pathf, &var->statbuf) == -1)
		return (free(var->pathf), NULL);
	if (S_ISDIR(var->statbuf.st_mode))
	{
		if (!ft_strncmp(var->f, var->entry->d_name, ft_strlen(var->f)))
			return (ret = ft_strdup(var->pathf), freeffdr(var), ret);
		var->pathf = dir_seek(ft_strdup(var->f), var->pathf);
		if (var->pathf)
			return (ret = ft_strdup(var->pathf), freeffdr(var), ret);
	}
	free(var->pathf);
	var->pathf = NULL;
	return (NULL);
}

void	freeffdr(t_ffdr *var)
{
	closedir(var->dir);
	if (var->directory)
		free(var->directory);
	if (var->f)
		free(var->f);
	if (var->pathf)
		free(var->pathf);
	free(var);
}
