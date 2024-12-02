/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minish3.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jainavas <jainavas@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/12 21:06:20 by jainavas          #+#    #+#             */
/*   Updated: 2024/12/02 17:36:21 by jainavas         ###   ########.fr       */
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

int	ft_dstrchr(char **s, char *s2)
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

int	ft_dstrlen(char **s)
{
	int	i;

	i = 0;
	if (!s)
		return (0);
	while (s[i])
		i++;
	return (i);
}

char	*debuginout(char *buf2, t_mini *mini)
{
	char	*tmp;
	int		i;

	tmp = NULL;
	tmp = ft_strchr(buf2, '<');
	i = 1;
	if (tmp != NULL && tmp[1] != '<')
	{
		while (tmp[i] == ' ')
			i++;
		while (ft_isalnum(tmp[i]) || tmp[i] == '.')
			i++;
		tmp = ft_substr(buf2, (tmp + 1) - buf2, i);
		mini->infile = ft_strtrim(tmp, " ");
		free(tmp);
		tmp = ft_substr(buf2, i, ft_strlen(buf2));
		free(buf2);
		buf2 = ft_strtrim(tmp, " ");
		free(tmp);
	}
	else
		mini->infile = ft_strdup("/dev/stdin");
	if (ft_strchr(buf2, '>') != NULL)
		return (handlemfilesout(mini, buf2),
				tmp = ft_substr(buf2, 0, ft_strchr(buf2, '>') - buf2),
				free(buf2), tmp);
	else
		return (newfileout(mini->mfilesout, ft_strdup("/dev/stdout"), 0), buf2);
}

int	dolimitonecmd(char **buf, t_mini *mini)
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
	freedoublepointer(cmd);
	free(mini->infile);
	return (0);
}
