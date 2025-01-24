/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   evaluator2.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jainavas <jainavas@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/20 18:52:43 by jainavas          #+#    #+#             */
/*   Updated: 2025/01/24 18:09:46 by mpenas-z         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

int	is_operator(char *buf)
{
	if (!buf)
		return (1);
	if (!ft_strncmp(buf, "|", 2) || !ft_strncmp(buf, "<", 2)
		|| !ft_strncmp(buf, ">", 2))
		return (1);
	return (0);
}

// WIP
// Also make sure cat < < lim WILL NOT WORK, probably is better to
//  split << as one.
int	check_operator_syntax(char **args)
{
	int	i;

	i = -1;
	while (args[++i])
	{
		if (is_operator(args[i]))
		{
			if (!args[i + 1])
				return (ft_putstr_fd("Syntax error\n", 2), 2);
		}
	}
	return (0);
}

void	free_cmd_list(t_cmd **head)
{
	t_cmd	*current;

	current = *head;
	while (*head)
	{
		current = current->next;
		free_cmd(*head);
		*head = current;
	}
}

void	free_cmd(t_cmd *cmd)
{
	if (cmd->cmd)
		free (cmd->cmd);
	if (cmd->argv)
		freedoublepointer(cmd->argv);
	if (cmd->infile)
		free (cmd->infile);
	if (cmd->lim)
		free (cmd->lim);
	if (cmd->env)
		freedoublepointer(cmd->env);
	if (cmd->path)
		free (cmd->path);
	if (cmd->outfiles)
	{
		freeoutfiles(cmd->outfiles);
		free(cmd->outfiles);
	}
	free (cmd);
}
