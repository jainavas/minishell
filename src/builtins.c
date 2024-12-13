/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jainavas <jainavas@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/10 12:14:17 by mpenas-z          #+#    #+#             */
/*   Updated: 2024/12/13 20:32:30 by jainavas         ###   ########.fr       */
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

// Need to avoid numbers or empty spaces to be variable names
void	doexport(t_mini *mini, char *buf)
{
	int		argc;
	char	**args;
	char	**parsed_line;

	args = ft_split(buf, ' ');
	argc = 0;
	while (args[++argc])
	{
		parsed_line = ft_split(args[argc], '=');
		if (!parsed_line[1])
			add_temp_envar(mini, parsed_line[0]);
		else
			add_envar(mini, parsed_line[0], parsed_line[1]);
		freedoublepointer(parsed_line);
	}
	if (argc == 1)
		print_temp_env(mini->env);
	freedoublepointer(args);
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
	if (argc == 1)
	{
		printf("unset: not enough arguments\n");
		return ;
	}
	else if (argc >= 2)
	{
		argc = 0;
		while (parsed_line[++argc])
			remove_envar(mini, parsed_line[argc]);
	}
	freedoublepointer(parsed_line);
}

// Need to solve that exportt works -> This is a parsing problem.
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
		entvars(&mini->env, ft_substr(buf2, 0,
				ft_strchr(buf2, '=') - buf2),
			ft_strdup(ft_strchr(buf2, '=') + 1));
		return (free(buf2), 0);
	}
	return (-1);
}
