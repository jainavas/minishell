/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fileshandle2.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jainavas <jainavas@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/30 16:38:08 by jainavas          #+#    #+#             */
/*   Updated: 2025/01/20 22:31:14 by jainavas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*fileseek(t_ffdr *var)
{
	char	*ret;

	if (!var->directory)
		return (freeffdr(var), NULL);
	var->dir = opendir(var->directory);
	if (!var->dir)
		return (freeffdr(var), NULL);
	var->entry = readdir(var->dir);
	while (var->entry)
	{
		ret = fileseekutils(var);
		if (ret)
			return (ret);
		var->entry = readdir(var->dir);
	}
	return (freeffdr(var), NULL);
}

char	*pathbuilder(char *dir, char *file)
{
	char	*res;

	res = ft_strjoin(dir, "/");
	res = ft_strjoin_gnl(res, file);
	return (res);
}

char	*dir_seek(char *target_dir, char *directory)
{
	t_ffdr	*var;
	char	*ret;

	var = ft_calloc(1, sizeof(t_ffdr));
	var->f = target_dir;
	var->directory = directory;
	var->dir = opendir(var->directory);
	if (!var->dir)
		return (freeffdr(var), NULL);
	var->entry = readdir(var->dir);
	while (var->entry)
	{
		ret = dirseekutils(var);
		if (ret)
			return (ret);
		var->entry = readdir(var->dir);
	}
	return (freeffdr(var), NULL);
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

int	seekcasebar(t_fout *tmp, char *tmp2, char *tmp3, t_mini *mini)
{
	t_ffdr	*var;

	var = ft_calloc(1, sizeof(t_ffdr));
	var->directory = dir_seek(ft_strndup(tmp2, ft_strrchr(tmp2, '/') - tmp2),
			getcwd(NULL, 0));
	var->f = ft_strdup(&tmp2[(ft_strrchr(tmp2, '/') - tmp2) + 1]);
	tmp3 = fileseek(var);
	if (tmp3 == NULL)
	{
		tmp3 = dir_seek(ft_strndup(tmp2, ft_strrchr(tmp2, '/') - tmp2),
				getcwd(NULL, 0));
		if (!tmp3)
			return (free(tmp2), tmp->out = -2,
				ft_putendl_fd("File not found", 2), mini->status = 1, 0);
		else
			return (free(tmp2), free(tmp3), 0);
	}
	else
		return (free(tmp2), free(tmp->file), tmp->file = tmp3, 0);
}
