/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   evaluator2.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jainavas <jainavas@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/20 18:52:43 by jainavas          #+#    #+#             */
/*   Updated: 2025/01/27 17:21:29 by mpenas-z         ###   ########.fr       */
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

int	is_in_out_file(char **args, int i)
{
	if (i <= 0)
		return (0);
	if (!args[i] || !args[i - 1])
		return (0);
	if (!ft_strncmp(args[i - 1], "<", 2) || !ft_strncmp(args[i - 1], ">", 2))
		return (1);
	return (0);
}

/*void	print_cmd_list(t_cmd *head)*/
/*{*/
/*	t_cmd	*current;*/
/*	t_fout	*outf;*/
/*	int		i;*/
/*	printf("Printing cmd list: %p\n", head);*/
/*	current = head;*/
/*	while (current)*/
/*	{*/
/*		if (current)*/
/*		{*/
/*			outf = *current->outfiles;*/
/*			if (!current->cmd)*/
/*				return ;*/
/*			printf("Cmd: %s\n", current->cmd);*/
/*			printf("Argv: {");*/
/*			i = -1;*/
/*			while (current->argv[++i])*/
/*				printf("%s, ", current->argv[i]);*/
/*			if (current->argv[i] == NULL)*/
/*				printf("(nil)");*/
/*			printf("}\n");*/
/*			printf("Argc: %d\n", current->argc);*/
/*			printf("Lim: %s\n", current->lim);*/
/*			while (outf)*/
/*			{*/
/*				printf("Outfile: %s\n", outf->file);*/
/*				outf = outf->next;*/
/*			}*/
/*			printf("Infile: %s\n", current->infile);*/
/*		}*/
/*		current = current->next;*/
/*	}*/
/*}*/

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
			if (!args[i + 1] || is_operator(args[i + 1]))
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
