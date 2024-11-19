/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minish3.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jainavas <jainavas@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/12 21:06:20 by jainavas          #+#    #+#             */
/*   Updated: 2024/11/19 17:33:01 by jainavas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini.h"

void	docd(char *path)
{
	if (access(path, F_OK) == 0)
		chdir(path);
	else
		printf("cd: no such file or directory: %s\n", path);
}

int ft_dstrchr(char **s, char *s2)
{
	int	i;

	i = -1;
	if (!s || !s2)
		return (-1);
	while (s[++i])
	{
		if (ft_strncmp(s[i], s2, ft_strlen(s[i])) == 0)
				return (i);
	}
	return (-1);
}

int ft_dstrlen(char **s)
{
	int	i;

	i = 0;
	if (!s)
		return (0);
	while (s[i])
		i++;
	return (i);
}

void	debuginout(char *buf2, char **buf, t_mini *mini)
{
	if (ft_strchr(buf2, '<') != NULL && ft_strchr(buf2, '<')[1] != '<')
		mini->infile = buf[0];
	else
		mini->infile = ft_strdup("/dev/stdin");
	if (ft_strchr(buf2, '>') != NULL)
		mini->fileout = buf[ft_dstrlen(buf) - 1];
	else
		mini->fileout = ft_strdup("/dev/stdout");
	mini->out = 0;
	if (ft_strchr(buf2, '>') != NULL)
		mini->out = 1;
	mini->appendout = 0;
	if (ft_strchr(buf2, '>') != NULL && ft_strchr(buf2, '>')[1] == '>')
		mini->appendout = 1;
}

int dolimitonecmd(char **buf, t_mini *mini)
{
	int		fd;
	char	*buf2;
	char	**cmd;

	fd = open("tmp_heredoc", O_CREAT | O_WRONLY, 0777);
	limmitator(buf[1], fd);
	close(fd);
	fd = open("tmp_heredoc", O_RDONLY);
	cmd = ft_split(buf[0], ' ');
	alonecmdcall(fd, cmd, pathseek(cmd, mini->envp), mini);
	close(fd);
	buf2 = NULL;
	buf2 = ft_strjoin_gnl(getcwd(buf2, 0), "/tmp_heredoc");
	unlink(buf2);
	free(buf2);
	freedoublepointer(buf);
	free(mini->infile);
	return (0);
}