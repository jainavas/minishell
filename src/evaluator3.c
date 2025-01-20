/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   evaluator3.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jainavas <jainavas@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/20 18:53:08 by jainavas          #+#    #+#             */
/*   Updated: 2025/01/20 19:22:34 by jainavas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

char	*argsearch(char *file)
{
	char	*tmp2;
	char	*tmp3;

	tmp2 = ft_strtrim(file, " ./\"");
	if (ft_strchr(tmp2, '/'))
	{
		tmp3 = fileseek(ft_strdup(&tmp2[(ft_strrchr(tmp2, '/') - tmp2) + 1]),
				dir_seek(ft_strndup(tmp2, ft_strrchr(tmp2, '/') - tmp2),
					getcwd(NULL, 0)));
		if (tmp3 == NULL)
		{
			tmp3 = dir_seek(ft_strndup(tmp2, ft_strrchr(tmp2, '/') - tmp2),
					getcwd(NULL, 0));
			if (!tmp3)
				return (free(tmp2), file);
			else
				return (tmp3 = ft_strjoin_gnl(tmp3,
						&tmp2[(ft_strrchr(tmp2, '/') - tmp2) + 1]),
					free(tmp2), tmp3);
		}
		else
			return (free(tmp2), free(file), tmp3);
	}
	else
	{
		tmp3 = fileseek(tmp2, getcwd(NULL, 0));
		if (tmp3 == NULL)
			return (tmp2 = ft_strtrim(file, " ./\""), free(file),
				tmp3 = ft_strjoin_gnl(getcwd(NULL, 0), "/"),
				tmp3 = ft_strjoin_gnl(tmp3, tmp2), free(tmp2), tmp3);
		else
			return (free(file), tmp3);
	}
	return (0);
}

void	argsfilesearcher(t_cmd **head)
{
	t_cmd	*tmp;
	int		i;

	tmp = *head;
	while (tmp)
	{
		i = -1;
		while (tmp->argv[++i])
		{
			if (tmp->argv[i] && !strncmp("./", tmp->argv[i], 2))
				tmp->argv[i] = argsearch(tmp->argv[i]);
		}
		if (tmp->infile && !strncmp("./", tmp->infile, 2))
			tmp->infile = argsearch(tmp->infile);
		tmp = tmp->next;
	}
}
