/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   evaluator.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mpenas-z <mpenas-z@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/28 18:05:55 by mpenas-z          #+#    #+#             */
/*   Updated: 2024/12/28 20:15:42 by mpenas-z         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/mini.h"

t_cmd	*evaluate_commands(char **args)
{
	t_cmd	*head;
	t_cmd	*current;
	int		cmd_count;
	int		argc;
	int		i;

	argc = 0;
	i = -1;
	while (args[++i])
	{
		if (!is_operator(args[i]))
		{
			argc = i;
			while (!is_operator(args[argc]))
				argc++;
			argc = argc - i;
			if (!current)
				current = new_command(args, argc + i, argc);
			else
			{
				current->next = new_command(args, argc + i, argc);
				current = current->next;
			}
			if (!head)
				head = current;
			if (!ft_strncmp(args[argc + i], "<", 1))
			{
				if (!ft_strncmp(args[argc + ++i], "<", 1))
					current->lim = args[argc + ++i];
				current->infile = open(args[argc + i], O_RDONLY);
				if (current->infile == -1)
					perror("open");
			}
			else if (!ft_strncmp(args[argc + i], ">", 1))
			{
				current->outfile = open(args[argc + i], \
										O_WRONLY | O_CREAT | O_TRUNC, 0644);
				if (current->outfile == -1)
					perror("open");
			}
		}
	}
	return (head);
}

int	is_operator(char *buf)
{
	if (!ft_strncmp(buf, "|", 1) || !ft_strncmp(buf, "<", 1)
		|| !ft_strncmp(buf, ">", 1))
		return (1);
	return (0);
}

t_cmd	*new_command(char *buf)
{
	t_cmd	*cmd;

	cmd = ft_calloc(1, sizeof(t_cmd));
	cmd->cmd = buf;
	cmd->argv = NULL;
	cmd->lim = NULL;
	cmd->argc = 1;
	cmd->infile = -1;
	cmd->outfile = -1;
	cmd->next = NULL;
	return (cmd);
}
