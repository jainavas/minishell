/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minish6.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jainavas <jainavas@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/28 17:07:05 by jainavas          #+#    #+#             */
/*   Updated: 2025/01/10 18:55:10 by jainavas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./minishell.h"

t_fout	*foutlast(t_fout *lst)
{
	t_fout	*tmp;

	tmp = lst;
	if (lst)
		while (tmp->next)
			tmp = tmp->next;
	return (tmp);
}

void	handlemfilesout(t_mini *mini, char *buf)
{
	char	*tmp;
	int		i;
	int		a;

	tmp = ft_strchr(buf, '>');
	i = 1;
	a = 0;
	while (tmp)
	{
		if (tmp[1] == '>')
		{
			a = 1;
			tmp++;
		}
		while (ft_isalnum(tmp[i]) || tmp[i] == '.' || tmp[i] == ' ')
			i++;
		newfileout(mini->mfilesout, ft_substr(tmp, 0, i), a);
		i = 1;
		tmp = ft_strchr(tmp + 1, '>');
	}
}
// fix the ./ op by getcwd into . correctly
void	fdtomfiles(t_fout **head, int fd, t_mini *mini)
{
	t_fout	*tmp;

	tmp = *head;
	while (tmp && tmp->next)
	{
		if (tmp->file[0] == '.')
			tmp->file = ft_strinsertdup(tmp->file, ".", ft_strjoin_gnl(getcwd(NULL, 0), "/"), '.');
		anyfdtofile(-1, tmp->file, tmp->appendout, mini);
		tmp = tmp->next;
	}
	ft_putnbr_fd(fd, fd);
	if (tmp->file[0] == '.')
			tmp->file = ft_strinsertdup(tmp->file, ".", getcwd(NULL, 0), '.');
	anyfdtofile(fd, tmp->file, tmp->appendout, mini);
	freeoutfiles(head);
}

void	freeoutfiles(t_fout **lst)
{
	t_fout	*new;

	while (*lst != NULL)
	{
		new = *lst;
		*lst = new->next;
		free(new->file);
		free(new);
	}
}

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
			return (NULL);
		tmp[ft_strlen(tmp) - 1] = '\0';
		return (close(fd[READ_FD]), tmp);
	}
	return (NULL);
}
