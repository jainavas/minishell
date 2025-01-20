/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   evaluator.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jainavas <jainavas@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/28 18:05:55 by mpenas-z          #+#    #+#             */
/*   Updated: 2025/01/20 19:25:59 by jainavas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

t_cmd	*evaluate_commands(char **args)
{
	t_cmd	*head;
	t_cmd	*current;
	int		tmp;
	int		i;

	if (check_operator_syntax(args) != 0)
		return (NULL);
	head = NULL;
	current = NULL;
	i = -1;
	tmp = -1;
	while (args[++i])
	{
		if (args[i][0] == '|')
			current = NULL;
		if (!is_operator(args[i]))
		{
			if (!current)
			{
				cmdadd_back(&head, get_current_cmd(args, &i));
				current = cmdlast(head);
			}
			else
				assignarg(&current, args, &i);
			if (tmp != -1)
			{
				assign_infile(&current, args, &tmp);
				tmp = -1;
			}
		}
		else if (!ft_strncmp(args[i], "<", 1))
		{
			if (current)
				assign_infile(&current, args, &i);
			else
				tmp = i++;
		}
		else if (!ft_strcmpspace(args[i], ">")
			&& ft_strcmpspace(args[i + 1], ">"))
			assign_outfile(&current, args, &i, 0);
		else if (!ft_strncmp(args[i], ">", 1)
			&& !ft_strncmp(args[i + 1], ">", 1))
		{
			i++;
			assign_outfile(&current, args, &i, 1);
		}
	}
	return (putcmdn(&head), argsfilesearcher(&head), head);
}

void	assign_outfile(t_cmd **current, char **args, int *begin, int app)
{
	int		i;

	if (!(*current))
		return ;
	i = *begin;
	if (!args[++i])
		return ;
	else if (args[i] && !is_operator(args[i]))
		newfileout((*current)->outfiles, ft_strdup(args[i]), app, i);
	*begin = i;
}

void	assign_infile(t_cmd **current, char **args, int *begin)
{
	int		i;

	if (!(*current))
		return ;
	i = *begin;
	if (!args[++i])
		return ;
	if (args[i] && !is_operator(args[i]))
	{
		if ((*current)->infile)
			free((*current)->infile);
		(*current)->infile = ft_strdup(args[i]);
		(*current)->priorinflim = 1;
	}
	else if (args[++i])
	{
		if ((*current)->lim)
			free((*current)->lim);
		(*current)->lim = ft_strdup(args[i]);
		(*current)->priorinflim = 2;
	}
	(*current)->priorinfout = i;
	*begin = i;
}

t_cmd	*get_current_cmd(char **args, int *begin)
{
	t_cmd	*cmd;
	int		argc;
	int		i;

	argc = 0;
	while (args[*begin + argc] && !is_operator(args[*begin + argc]))
		argc++;
	cmd = ft_calloc(1, sizeof(t_cmd));
	cmd->argc = argc;
	cmd->ifouts = 0;
	cmd->priorinflim = 0;
	cmd->lim = NULL;
	cmd->oginput = NULL;
	cmd->next = NULL;
	cmd->prev = NULL;
	cmd->infile = NULL;
	cmd->outfiles = ft_calloc(1, sizeof(t_fout *));
	cmd->cmd = ft_strdup(args[*begin]);
	cmd->path = NULL;
	cmd->argv = ft_calloc(argc + 1, sizeof(char *));
	i = -1;
	while (++i < argc && args[*begin + i])
		cmd->argv[i] = ft_strdup(args[*begin + i]);
	cmd->argv[i] = NULL;
	*begin = *begin + argc - 1;
	return (cmd);
}

void	assignarg(t_cmd **cmd, char **args, int *begin)
{
	char	**newargv;
	int		argc;
	int		i;

	argc = 0;
	while (args[*begin + argc] && !is_operator(args[*begin + argc]))
		argc++;
	newargv = ft_calloc(argc + (*cmd)->argc + 2, sizeof(char *));
	i = -1;
	while ((*cmd)->argv[++i])
		newargv[i] = ft_strdup((*cmd)->argv[i]);
	freedoublepointer((*cmd)->argv);
	i = -1;
	while (++i < argc + (*cmd)->argc && args[*begin + i]
		&& !is_operator(args[*begin + i]))
		newargv[i + (*cmd)->argc] = ft_strdup(args[*begin + i]);
	newargv[i + (*cmd)->argc] = NULL;
	(*cmd)->argv = newargv;
	(*cmd)->argc = i + (*cmd)->argc;
	*begin = *begin + i - 1;
}
