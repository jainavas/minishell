/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   evaluator.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jainavas <jainavas@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/28 18:05:55 by mpenas-z          #+#    #+#             */
/*   Updated: 2025/01/06 03:49:24 by jainavas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/mini.h"

// theres a problem with infile handle when <Makefile grep a,
// tries to assign infile but the cmd struct doesnt exist yet coz is created when cmd is found

// theres two options in my op, or we just strdup it somewhere and do the assign infile after cmd,
// or we create it elsewhere
t_list	*evaluate_commands(char **args)
{
	t_list	*head;
	t_list	*current;
	int		i;

	if (check_operator_syntax(args) != 0)
		return (NULL);
	head = NULL;
	i = -1;
	while (args[++i])
	{
		if (head)
			current = ft_lstlast(head);
		if (!is_operator(args[i]))
		{
			ft_lstadd_back(&head, ft_lstnew(NULL));
			current = ft_lstlast(head);
			current->content = get_current_cmd(args, &i); // HERE, if not current->content is uninitialized
		}
		else if (!ft_strncmp(args[i], "<", 1))
			current = assign_infile(&current, args, &i);
		else if (!ft_strncmp(args[i], ">", 1) && ft_strncmp(args[i + 1], ">", 1))
			current = assign_outfile(&current, args, &i, 0);
		else if (!ft_strncmp(args[i], ">", 1) && !ft_strncmp(args[i + 1], ">", 1))
		{
			i++;
			current = assign_outfile(&current, args, &i, 1);
		}
	}
	return (head);
}

t_list	*assign_outfile(t_list **current, char **args, int *begin, int app)
{
	int		i;
	t_cmd	*cmd;

	if (!(*current))
		return (NULL);
	cmd = (t_cmd *)(*current)->content;
	i = *begin;
	if (!args[++i])
		return (*current);
	else if (args[i] && !is_operator(args[i]))
		newfileout(cmd->outfiles, ft_strdup(args[i]), app);
	(*current)->content = cmd;
	*begin = i;
	return ((*current));
}

t_list	*assign_infile(t_list **current, char **args, int *begin)
{
	int		i;
	t_cmd	*cmd;

	if (!(*current))
		return (NULL);
	cmd = (t_cmd *)(*current)->content;
	i = *begin;
	if (!args[++i])
		return ((*current));
	if (args[i] && !is_operator(args[i]))
		cmd->infile = ft_strdup(args[i]);
	else if (args[++i])
		cmd->lim = ft_strdup(args[i]);
	*begin = i;
	(*current)->content = cmd;
	return ((*current));
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
	cmd->lim = NULL;
	cmd->infile = NULL;
	cmd->outfiles = ft_calloc(1, sizeof(t_fout *));
	cmd->cmd = ft_strdup(args[*begin]);
	cmd->argv = ft_calloc(argc + 1, sizeof(char *));
	i = -1;
	while (++i < argc && args[*begin + i])
		cmd->argv[i] = ft_strdup(args[*begin + i]);
	cmd->argv[i] = NULL;
	*begin = *begin + argc - 1;
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
// Should we implement stuff like "cat < infile < infile2" or
// "echo hola > outfile > outfile2"? -> If so do it in assign_outfile;
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
	t_fout	*outf;
	int		i;

	printf("Printing cmd list: %p\n", head);
	current = head;
	while (current)
	{
		if (current->content)
		{
			cmd = (t_cmd *)current->content;
			outf = *cmd->outfiles;
			if (!cmd)
				return ;
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
			while (outf)
			{
				printf("Outfile: %s\n", outf->file);
				outf = outf->next;
			}
			printf("Infile: %s\n", cmd->infile);
		}
		current = current->next;
	}
}

// Need to strdup from args when copying argv.
void	free_cmd_list(t_list *head)
{
	t_list	*current;

	current = head;
	while (head)
	{
		free_cmd(head->content);
		current = current->next;
		free (head);
		head = current;
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
	if (cmd->outfiles)
	{
		freeoutfiles(cmd->outfiles);
		free(cmd->outfiles);
	}
	free (cmd);
}
