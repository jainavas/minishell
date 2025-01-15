/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   evaluator.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jainavas <jainavas@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/28 18:05:55 by mpenas-z          #+#    #+#             */
/*   Updated: 2025/01/15 19:05:29 by jainavas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

// theres a problem with infile handle when <Makefile grep a,
// tries to assign infile but the cmd struct doesnt exist yet coz is created when cmd is found

// theres two options in my op, or we just strdup it somewhere and do the assign infile after cmd,
// or we create it elsewhere
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
				cmdadd_back(&head, get_current_cmd(args, &i)); // HERE, if not current->content is uninitialized
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
		else if (!ft_strcmpspace(args[i], ">") && ft_strcmpspace(args[i + 1], ">"))
			assign_outfile(&current, args, &i, 0);
		else if (!ft_strncmp(args[i], ">", 1) && !ft_strncmp(args[i + 1], ">", 1))
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
		newfileout((*current)->outfiles, ft_strdup(args[i]), app);
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
		(*current)->infile = ft_strdup(args[i]);
	else if (args[++i])
		(*current)->lim = ft_strdup(args[i]);
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
// Also make sure cat < < lim WILL NOT WORK, probably is better to split << as one.
// It automatically shall call mini_error system on error, otherwise return 0.
int	check_operator_syntax(char **args)
{
	(void)args;
	return (0);
}

void	print_cmd_list(t_cmd *head)
{
	t_cmd	*current;
	t_fout	*outf;
	int		i;

	printf("Printing cmd list: %p\n", head);
	current = head;
	while (current)
	{
		if (current)
		{
			outf = *current->outfiles;
			if (!current->cmd)
				return ;
			printf("Cmd: %s\n", current->cmd);
			printf("Argv: {");
			i = -1;
			while (current->argv[++i])
				printf("%s, ", current->argv[i]);
			if (current->argv[i] == NULL)
				printf("(nil)");
			printf("}\n");
			printf("Argc: %d\n", current->argc);
			printf("Lim: %s\n", current->lim);
			while (outf)
			{
				printf("Outfile: %s\n", outf->file);
				outf = outf->next;
			}
			printf("Infile: %s\n", current->infile);
		}
		current = current->next;
	}
}

// Need to strdup from args when copying argv.
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
	if (cmd->oginput)
		free (cmd->oginput);
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

void	assignarg(t_cmd **cmd, char **args, int *begin)
{
	int		argc;
	int		i;

	argc = 0;
	while (args[*begin + argc] && !is_operator(args[*begin + argc]))
		argc++;
	freedoublepointer((*cmd)->argv);
	(*cmd)->argv = ft_calloc(argc + 2, sizeof(char *));
	i = 0;
	(*cmd)->argv[0] = ft_strdup((*cmd)->cmd);
	while (++i <= argc && args[*begin + i - 1])
		(*cmd)->argv[i] = ft_strdup(args[*begin + i - 1]);
	(*cmd)->argv[i] = NULL;
}

//WIP

char	*argsearch(char *file)
{
	char	*tmp2;
	char	*tmp3;

	tmp2 = ft_strtrim(file, " ./\"");
	if (ft_strchr(tmp2, '/'))
	{
		tmp3 = fileseek(ft_strdup(&tmp2[(ft_strrchr(tmp2, '/') - tmp2) + 1]),
			directory_seek(ft_strndup(tmp2, ft_strrchr(tmp2, '/') - tmp2),
				getcwd(NULL, 0)));
		if (tmp3 == NULL)
		{
			tmp3 = directory_seek(ft_strndup(tmp2, ft_strrchr(tmp2, '/') - tmp2),
				getcwd(NULL, 0));
			if (!tmp3)
				return (free(tmp2), file);
			else
				return (tmp3 = ft_strjoin_gnl(tmp3, &tmp2[(ft_strrchr(tmp2, '/') - tmp2) + 1]), free(tmp2), tmp3);
		}
		else
			return (free(tmp2), free(file), tmp3);
	}
	else
	{
		tmp3 = fileseek(tmp2, getcwd(NULL, 0));
		if (tmp3 == NULL)
			return (tmp2 = ft_strtrim(file, " ./\""), free(file), tmp3 = ft_strjoin_gnl(getcwd(NULL, 0), "/"), tmp3 = ft_strjoin_gnl(tmp3, tmp2), free(tmp2), tmp3);
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
