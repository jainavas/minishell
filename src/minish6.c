/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minish6.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jainavas <jainavas@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/28 17:07:05 by jainavas          #+#    #+#             */
/*   Updated: 2025/01/20 16:26:30 by jainavas         ###   ########.fr       */
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
		newfileout(mini->mfilesout, ft_substr(tmp, 0, i), a, 0);
		i = 1;
		tmp = ft_strchr(tmp + 1, '>');
	}
}

void	fdtomfiles(t_fout **head, int fd, t_mini *mini, t_cmd *cmd)
{
	t_fout	*tmp;

	tmp = *head;
	while (tmp)
	{
		if (tmp->file[0] == '.')
			filesearch(tmp, mini);
		if (tmp->out != -2)
			if (anyfdtofile(fd, tmp, cmd, mini) == -1)
				break ;
		tmp = tmp->next;
	}
	freeoutfiles(head);
}

int	filesearch(t_fout *tmp, t_mini *mini)
{
	char	*tmp2;
	char	*tmp3;

	tmp2 = ft_strtrim(tmp->file, " ./");
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
				return (free(tmp2), tmp->out = -2, ft_putendl_fd("File not found", 2), mini->status = 1, 0);
			else
				return (free(tmp2), free(tmp3), 0);
		}
		else
			return (free(tmp2), free(tmp->file), tmp->file = tmp3, 0);
	}
	else
	{
		tmp3 = fileseek(tmp2, getcwd(NULL, 0));
		if (tmp3 == NULL)
			return (ft_putendl_fd("File not found", 2), mini->status = 1, 0);
		else
			return (free(tmp->file), tmp->file = tmp3, 0);
	}
	return (0);
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
			return (close(fd[READ_FD]), NULL);
		tmp[ft_strlen(tmp) - 1] = '\0';
		return (close(fd[READ_FD]), tmp);
	}
	return (NULL);
}
