/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   evaluator.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mpenas-z <mpenas-z@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/28 18:05:55 by mpenas-z          #+#    #+#             */
/*   Updated: 2024/12/29 00:32:54 by mpenas-z         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/mini.h"

t_list	*evaluate_commands(char **args)
{
	t_list	*head;
	t_list	*current;
	t_cmd	*cmd;
	int		argc;
	int		i;

	if (check_operator_syntax(args) != 0)
		return (NULL);
	head = NULL;
	i = -1;
	while (args[++i])
	{
		if (ft_strncmp(args[i], "|", 1))
		{
			ft_lstadd_back(&head, ft_lstnew(NULL));
			current = ft_lstlast(head);
			argc = 0;
			while (args[i + argc] && !is_operator(args[i + argc]))
			{
				argc++;
				printf("ARGC: %d\n", argc);
			}
			current->content = get_current_cmd(args, i, argc);
			cmd = (t_cmd *)current->content;
			if (cmd->lim != NULL)
				i = i + argc + 2;
			else if (cmd->infile != NULL
					|| cmd->outfile != NULL)
				i = i + argc + 1;
			else
				i = i + argc;
		}
	}
	return (head);
}

t_cmd	*get_current_cmd(char **args, int begin, int argc)
{
	t_cmd	*cmd;
	int		i;

	cmd = ft_calloc(1, sizeof(t_cmd));
	cmd->argc = argc;
	cmd->lim = NULL;
	cmd->infile = NULL;
	cmd->outfile = NULL;
	cmd->cmd = args[begin];
	cmd->argv = ft_calloc(argc + 1, sizeof(char *));
	i = -1;
	while (++i < argc && args[begin + i])
		cmd->argv[i] = args[begin + i];
	cmd->argv[i] = NULL;
	if (args[begin + argc] && !ft_strncmp(args[begin + argc], ">", 1))
		cmd->outfile = args[begin + argc + 1];
	else if (args[begin + argc] && !ft_strncmp(args[begin + argc], "<", 1))
	{
		if (args[begin + argc + 1] && !ft_strncmp(args[begin + argc + 1], "<", 1))
			cmd->lim = args[begin + argc + 2];
		else
			cmd->infile = args[begin + argc + 1];
	}
	return (cmd);
}

int	is_operator(char *buf)
{
	if (!buf)
		return (1);
	if (!ft_strncmp(buf, "|", 1) || !ft_strncmp(buf, "<", 1)
		|| !ft_strncmp(buf, ">", 1))
		return (1);
	return (0);
}

// WIP
// Need to look for syntax errors like: "echo hello >", "cat <", or "cat |".
// Should we implement stuff like "cat < infile < infile2" or "echo hola > outfile > outfile2"?
// Also make sure cat < < lim WILL NOT WORK, probably is better to split << as one.
// It automatically shall call mini_error system on error, otherwise return 0.
int	check_operator_syntax(char **args)
{
	(void)args;
	return (0);
}

void	print_cmd_list(t_list *head)
{
	t_list	*current;
	t_cmd	*cmd;
	int		i;

	printf("Printing cmd list: %p\n", head);
	current = head;
	while (current)
	{
		if (current->content)
		{
			cmd = (t_cmd *)current->content;
			printf("Cmd: %s\n", cmd->cmd);
			printf("Argv: {");
			i = -1;
			while (cmd->argv[++i])
				printf("%s, ", cmd->argv[i]);
			if (cmd->argv[i] == NULL)
				printf("(nil)");
			printf("}\n");
			printf("Argc: %d\n", cmd->argc);
			printf("Lim: %s\n", cmd->lim);
			printf("Outfile: %s\n", cmd->outfile);
			printf("Infile: %s\n", cmd->infile);
		}
		current = current->next;
	}
}
