/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   evaluator2.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jainavas <jainavas@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/20 18:52:43 by jainavas          #+#    #+#             */
/*   Updated: 2025/01/20 20:14:32 by jainavas         ###   ########.fr       */
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
// Need to look for syntax errors like: "echo hello >", "cat <", or "cat |".
// Should we implement stuff like "cat < infile < infile2" or
// "echo hola > outfile > outfile2"? -> If so do it in assign_outfile;
// Also make sure cat < < lim WILL NOT WORK, probably is better to
//  split << as one.
// It automatically shall call mini_error system on error, otherwise return 0.
int	check_operator_syntax(char **args)
{
	(void)args;
	return (0);
}

// void	print_cmd_list(t_cmd *head)
// {
// 	t_cmd	*current;
// 	t_fout	*outf;
// 	int		i;

// 	printf("Printing cmd list: %p\n", head);
// 	current = head;
// 	while (current)
// 	{
// 		if (current)
// 		{
// 			outf = *current->outfiles;
// 			if (!current->cmd)
// 				return ;
// 			printf("Cmd: %s\n", current->cmd);
// 			printf("Argv: {");
// 			i = -1;
// 			while (current->argv[++i])
// 				printf("%s, ", current->argv[i]);
// 			if (current->argv[i] == NULL)
// 				printf("(nil)");
// 			printf("}\n");
// 			printf("Argc: %d\n", current->argc);
// 			printf("Lim: %s\n", current->lim);
// 			while (outf)
// 			{
// 				printf("Outfile: %s\n", outf->file);
// 				outf = outf->next;
// 			}
// 			printf("Infile: %s\n", current->infile);
// 		}
// 		current = current->next;
// 	}
// }

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
