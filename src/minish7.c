/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minish7.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jainavas <jainavas@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/30 16:38:08 by jainavas          #+#    #+#             */
/*   Updated: 2025/01/20 18:07:32 by jainavas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	counttmps(t_env *lst)
{
	t_env	*new;
	int		i;

	new = lst;
	i = 0;
	while (new != NULL)
	{
		if (strncmp("holatmp_", new->name, 8) == 0)
			i++;
		new = new->next;
	}
	return (i);
}

char	*fileseek(char *file, char *directory)
{
	DIR				*dir;
	struct dirent	*entry;
	struct stat		statbuf;
	char			*pathf;

	if (!directory)
		return (free(file), NULL);
	dir = opendir(directory);
	if (!dir)
		return (NULL);
	entry = readdir(dir);
	while (entry)
	{
		if (entry->d_name[0] == '.')
		{
			entry = readdir(dir);
			continue ;
		}
		pathf = pathbuilder(directory, entry->d_name);
		if (stat(pathf, &statbuf) == -1)
		{
			free(pathf);
            continue ;
		}
		if (S_ISDIR(statbuf.st_mode))
		{
			pathf = fileseek(ft_strdup(file), pathf);
			if (pathf)
				return (closedir(dir), free(directory), free(file), pathf);
		}
		else
		{
			if (!ft_strcmpff(pathbuilder(directory, file), pathbuilder(directory, entry->d_name)))
				return (closedir(dir), free(directory), free(file), pathf);
			free(pathf);
		}
		entry = readdir(dir);
	}
	return (closedir(dir), free(directory), free(file), NULL);
}

char	*pathbuilder(char *dir, char *file)
{
	char	*res;

	res = ft_strjoin(dir, "/");
	res = ft_strjoin_gnl(res, file);
	return (res);
}

char	*directory_seek(char *target_dir, char *directory)
{
	DIR				*dir;
	struct dirent	*entry;
	struct stat		statbuf;
	char			*path;

	dir = opendir(directory);
	if (!dir)
	    return (NULL);
	entry = readdir(dir);
	while (entry)
	{
		if (entry->d_name[0] == '.')
		{
			entry = readdir(dir);
			continue ;
		}
		path = pathbuilder(directory, entry->d_name);
		if (stat(path, &statbuf) == -1)
		{
			free(path);
			entry = readdir(dir);
			continue ;
		}
		if (S_ISDIR(statbuf.st_mode))
		{
			if (!ft_strncmp(target_dir, entry->d_name, ft_strlen(target_dir)))
				return (closedir(dir), free(directory), free(target_dir), path);
			path = directory_seek(ft_strdup(target_dir), path);
			if (path)
				return (closedir(dir), free(directory), free(target_dir), path);
		}
		free(path);
		entry = readdir(dir);
	}
	closedir(dir);
	free(directory);
	free(target_dir);
	return (NULL);
}

void	putcmdn(t_cmd **head)
{
	t_cmd	*tmp;
	int		i;

	tmp = *head;
	i = 0;
	while (tmp)
	{
		tmp->cmdn = ++i;
		if (*tmp->outfiles)
			tmp->ifouts = 1;
		putoutfn(tmp->outfiles);
		tmp = tmp->next;
	}
}

void	putoutfn(t_fout **head)
{
	t_fout	*tmp;
	int		i;

	tmp = *head;
	i = 0;
	while (tmp)
	{
		tmp->foutn = ++i;
		tmp = tmp->next;
	}
}
