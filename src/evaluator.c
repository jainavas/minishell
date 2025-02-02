/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   evaluator.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jainavas <jainavas@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/28 18:05:55 by mpenas-z          #+#    #+#             */
/*   Updated: 2025/02/02 12:07:17 by mpenas-z         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

t_cmd	*evaluate_commands(char **args, t_mini *mini)
{
	t_cmd	*head;
	t_cmd	*current;
	int		i;

	if (check_operator_syntax(args, mini) != 0)
		return (NULL);
	head = NULL;
	cmdadd_back(&head, get_new_cmd());
	i = -1;
	while (args[++i])
	{
		if (args[i][0] == '|')
			cmdadd_back(&head, get_new_cmd());
		current = cmdlast(head);
		if (!is_operator(args[i]) && !current->cmd && !is_in_out_file(args, i))
			assign_cmd(&current, args, &i, mini);
		else if (!is_operator(args[i]) && !is_in_out_file(args, i))
			assignarg(&current, args, &i, mini);
		else
			casenoopevals(args, &i, &current, mini);
	}
	return (putcmdn(&head), argsfilesearcher(&head), head);
}

void	assign_outfile(t_cmd **current, char **args, int *begin, t_mini *mini)
{
	int	i;
	int	app;

	app = 0;
	if (!(*current))
		return ;
	i = *begin;
	if (!args[++i])
		return ;
	if (args[i] && !ft_strncmp(args[i - 1], ">>", 3))
		app = 1;
	if (args[i] && !is_operator(args[i]))
	{
		args[i] = process_vars(mini, args[i]);
		newfileout((*current)->outfiles, ft_strdup(args[i]), app, i);
	}
	*begin = i;
}

void	assign_infile(t_cmd **current, char **args, int *begin, t_mini *mini)
{
	int	i;

	if (!(*current))
		return ;
	i = *begin;
	if (!args[++i])
		return ;
	if (args[i] && !ft_strncmp(args[i - 1], "<", 2))
	{
		if ((*current)->infile)
			free((*current)->infile);
		args[i] = process_vars(mini, args[i]);
		(*current)->infile = ft_strdup(args[i]);
		(*current)->priorinflim = 1;
	}
	else if (args[i] && !ft_strncmp(args[i - 1], "<<", 3))
	{
		if ((*current)->lim)
			free((*current)->lim);
		(*current)->lim = ft_strdup(args[i]);
		(*current)->priorinflim = 2;
	}
	(*current)->priorinfout = i;
	*begin = i;
}

void	assign_cmd(t_cmd **current, char **args, int *begin, t_mini *mini)
{
	int		argc;
	int		i;

	argc = 0;
	while (args[*begin + argc] && !is_operator(args[*begin + argc]))
		argc++;
	(*current)->argc = argc;
	args[*begin] = process_vars(mini, args[*begin]);
	(*current)->cmd = ft_strdup(args[*begin]);
	(*current)->argv = ft_calloc(argc + 1, sizeof(char *));
	i = -1;
	while (++i < argc && args[*begin + i] && !is_operator(args[*begin + i]))
	{
		args[*begin + i] = process_vars(mini, args[*begin + i]);
		(*current)->argv[i] = ft_strdup(args[*begin + i]);
	}
	(*current)->argv[i] = NULL;
	*begin = *begin + argc - 1;
}

void	assignarg(t_cmd **cmd, char **args, int *begin, t_mini *mini)
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
	{
		args[*begin + i] = process_vars(mini, args[*begin + i]);
		newargv[i + (*cmd)->argc] = ft_strdup(args[*begin + i]);
	}
	newargv[i + (*cmd)->argc] = NULL;
	(*cmd)->argv = newargv;
	(*cmd)->argc = i + (*cmd)->argc;
	*begin = *begin + i - 1;
}
