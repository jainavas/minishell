/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minish6.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jainavas <jainavas@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/28 17:07:05 by jainavas          #+#    #+#             */
/*   Updated: 2025/01/20 19:19:51 by jainavas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./minishell.h"

char	*pathseekenv(char **args, char **envp)
{
	int			fd[2];
	int			pid;
	char		*tmp;
	char *const	argv[] = {"which", args[0], NULL};

	if (pipe(fd) == -1)
		return (NULL);
	pid = fork();
	if (pid == 0)
	{
		closeanddupoutput(fd);
		if (execve("/usr/bin/which", argv, envp) == -1)
			return (NULL);
	}
	else
	{
		close(fd[WRITE_FD]);
		wait(NULL);
		tmp = get_next_line(fd[READ_FD]);
		if (!tmp)
			return (close(fd[READ_FD]), NULL);
		tmp[ft_strlen(tmp) - 1] = '\0';
		return (close(fd[READ_FD]), tmp);
	}
	return (NULL);
}

int	counttmps(t_env *lst)
{
	t_env	*new;
	int		i;

	new = lst;
	i = 0;
	while (new != NULL)
	{
		if (strncmp("holatmp_", new->name, 8) == 0)
			i++;
		new = new->next;
	}
	return (i);
}

void	putoutfn(t_fout **head)
{
	t_fout	*tmp;
	int		i;

	tmp = *head;
	i = 0;
	while (tmp)
	{
		tmp->foutn = ++i;
		tmp = tmp->next;
	}
}
