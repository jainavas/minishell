/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   evaluator3.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jainavas <jainavas@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/20 18:53:08 by jainavas          #+#    #+#             */
/*   Updated: 2025/02/02 11:47:25 by mpenas-z         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

char	*argsearch(char *file)
{
	t_ffdr	*var;
	char	*tp2;
	char	*tmp3;

	tmp3 = NULL;
	tp2 = ft_strtrim(file, " ./\"");
	var = ft_calloc(1, sizeof(t_ffdr));
	if (ft_strchr(tp2, '/'))
		return (caseargsearch(var, tp2, file, tmp3));
	else
	{
		var->f = tp2;
		var->directory = getcwd(NULL, 0);
		tmp3 = fileseek(var);
		if (tmp3 == NULL)
			return (tp2 = ft_strtrim(file, " ./\""), free(file),
				tmp3 = ft_strjoin_gnl(getcwd(NULL, 0), "/"),
				tmp3 = ft_strjoin_gnl(tmp3, tp2), free(tp2), tmp3);
		else
			return (free(file), tmp3);
	}
	return (freeffdr(var), NULL);
}

void	argsfilesearcher(t_cmd **head)
{
	t_cmd	*tmp;
	int		i;

	tmp = *head;
	while (tmp)
	{
		i = -1;
		while (tmp->argv && tmp->argv[++i])
		{
			if (tmp->argv[i] && !strncmp("./", tmp->argv[i], 2))
				tmp->argv[i] = argsearch(tmp->argv[i]);
		}
		if (tmp->infile && !strncmp("./", tmp->infile, 2))
			tmp->infile = argsearch(tmp->infile);
		tmp = tmp->next;
	}
}

char	*caseargsearch(t_ffdr *var, char *tp2, char *file, char *tmp3)
{
	var->f = ft_strdup(&tp2[(ft_strrchr(tp2, '/') - tp2) + 1]);
	var->directory = dir_seek(ft_strndup(tp2, ft_strrchr(tp2, '/') - tp2),
			getcwd(NULL, 0));
	tmp3 = fileseek(var);
	if (tmp3 == NULL)
	{
		tmp3 = dir_seek(ft_strndup(tp2, ft_strrchr(tp2, '/') - tp2),
				getcwd(NULL, 0));
		if (!tmp3)
			return (free(tp2), file);
		else
			return (tmp3 = ft_strjoin_gnl(tmp3,
					&tp2[(ft_strrchr(tp2, '/') - tp2) + 1]),
				free(tp2), free(file), tmp3);
	}
	else
		return (free(tp2), free(file), tmp3);
}

void	casenoopevals(char **args, int *i, t_cmd **current, t_mini *mini)
{
	if ((!ft_strncmp(args[*i], "<", 2) || !ft_strncmp(args[*i], "<<", 3)))
		assign_infile(current, args, i, mini);
	if ((!ft_strncmp(args[*i], ">", 2) || !ft_strncmp(args[*i], ">>", 3)))
		assign_outfile(current, args, i, mini);
}

t_cmd	*get_new_cmd(void)
{
	t_cmd	*cmd;

	cmd = ft_calloc(1, sizeof(t_cmd));
	cmd->argc = 0;
	cmd->ifouts = 0;
	cmd->priorinflim = 0;
	cmd->infile = NULL;
	cmd->outfiles = ft_calloc(1, sizeof(t_fout *));
	cmd->cmd = NULL;
	cmd->path = NULL;
	cmd->argv = NULL;
	return (cmd->next = NULL, cmd->prev = NULL, cmd->lim = NULL, cmd);
}
