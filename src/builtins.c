/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mpenas-z <mpenas-z@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/10 12:14:17 by mpenas-z          #+#    #+#             */
/*   Updated: 2024/12/13 19:03:26 by mpenas-z         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/mini.h"

void	docd(char *path)
{
	if (access(path, F_OK) == 0)
		chdir(path);
	else
		printf("cd: no such file or directory: %s\n", path);
}

void	doecho(char *buf)
{
	if (ft_strncmp(buf, "echo -n ", 8) != 0)
	{
		write(1, &buf[5], ft_strlen(&buf[5]));
		if (buf[ft_strlen(buf) - 1] != '\n')
			write(1, "\n", 1);
	}
	else
		write(1, &buf[8], ft_strlen(&buf[8]));
	free(buf);
}

// Needs rework to accept multiple variables
void	doexport(t_mini *mini, char *buf)
{
	int		argc;
	char	**parsed_line;

	parsed_line = ft_splitchars(buf, " =");
	argc = 0;
	while (parsed_line[argc])
		argc++;
	if (argc == 1)
		print_temp_env(mini->env);
	else if (argc == 2)
		add_temp_envar(mini, parsed_line[1]);
	else if (argc == 3)
		add_envar(mini, parsed_line[1], parsed_line[2]);
	freedoublepointer(parsed_line);
}

void	dounset(t_mini *mini, char *buf)
{
	int		argc;
	char	**parsed_line;

	(void)mini;
	parsed_line = ft_split(buf, ' ');
	argc = 0;
	while (parsed_line[argc])
		argc++;
	// Give error: unset: not enough arguments
	if (argc == 1)
		return ;
	else if (argc >= 2)
	{
		argc = 0;
		while (parsed_line[++argc])
			remove_envar(mini, parsed_line[argc]);
	}
	freedoublepointer(parsed_line);
}

// Need to solve that exportt works.
int	builtins(t_mini *mini, char *buf2)
{
	if (checkkill(buf2))
		return (free(buf2), rl_clear_history(), 1);
	if (ft_strncmp("cd ", buf2, 3) == 0)
		return (buf2 = checkenvvars(buf2, mini), docd(&buf2[3]), free(buf2), 0);
	if (ft_strncmp("export", buf2, 6) == 0)
		return (doexport(mini, buf2), free(buf2), 0);
	if (ft_strncmp("unset", buf2, 5) == 0)
		return (dounset(mini, buf2), free(buf2), 0);
	if (ft_strncmp("env", buf2, 3) == 0)
		return (print_env(mini->env), free(buf2), 0);
	if (ft_strncmp("$?", buf2, 2) == 0)
		return (ft_putnbr_fd(g_status, 1), write(1, "\n", 1), free(buf2), 0);
	if (ft_strncmp("echo ", buf2, 5) == 0)
		return (buf2 = checkenvvars(buf2, mini), doecho(buf2), 0);
	if (ft_strchr(buf2, '=') && ft_strchr(buf2, '=')[-1] != ' '
		&& ft_strchr(buf2, '=')[1] != ' ')
	{
		buf2 = checkenvvars(buf2, mini);
		entvars(mini->env, ft_substr(buf2, 0,
				ft_strchr(buf2, '=') - buf2),
			ft_strdup(ft_strchr(buf2, '=') + 1));
		return (free(buf2), 0);
	}
	return (-1);
}
