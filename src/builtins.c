/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jainavas <jainavas@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/10 12:14:17 by mpenas-z          #+#    #+#             */
/*   Updated: 2024/12/17 23:44:14 by mpenas-z         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/mini.h"

void	docd(char *path, t_mini *mini)
{
	t_env	*tmp;
	char	*str;

	if (path[0] == '\0' || path[1] == '\0')
	{
		tmp = get_env_var(&mini->env, "HOME");
		chdir(tmp->content);
	}
	else if (path[1] == '~')
	{
		tmp = get_env_var(&mini->env, "HOME");
		str = ft_strinsertdup(strdup(path + 1), "" , tmp->content, '~');
		chdir(str);
		free(str);
	}
	else if (access(&path[1], F_OK) == 0)
		chdir(&path[1]);
	else
		ft_printf("cd: no such file or directory: %s\n", &path[1]);
}

void	doecho(char *buf)
{
	if (ft_strcmpspace(buf, "echo -n") != 0)
	{
		write(1, &buf[5], ft_strlen(&buf[5]));
		if (buf[ft_strlen(buf) - 1] != '\n')
			write(1, "\n", 1);
	}
	else
		write(1, &buf[8], ft_strlen(&buf[8]));
	free(buf);
}

int	is_bad_assignment(char *buf)
{
	int	i;

	i = -1;
	while (buf[++i])
		if (buf[i] == '=' && (!buf[i - 1] || buf[i - 1] == ' ') 
			&& (!buf[i + 1] || buf[i + 1] == ' '))
			return (1);
	return (0);
}

void	doexport(t_mini *mini, char *buf)
{
	int		argc;
	char	**args;
	char	**parsed_line;

	args = ft_split(buf, ' ');
	argc = 0;
	if (is_bad_assignment(buf) == 1)
	{
		printf("export: bad assignment\n");
		return ;
	}
	while (args[++argc])
	{
		parsed_line = ft_split(args[argc], '=');
		if (parsed_line[0][0] > '0' && parsed_line[0][0] < '9')
			printf("export: not an identifier: %c\n", parsed_line[0][0]);
		else if (!parsed_line[1])
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

int	builtins(t_mini *mini, char *buf2)
{
	if (checkkill(buf2))
		return (free(buf2), rl_clear_history(), 1);
	if (ft_strcmpspace("cd", buf2) == 0)
		return (buf2 = checkenvvars(buf2, mini), docd(&buf2[2], mini), free(buf2), 0);
	if (ft_strcmpspace("export", buf2) == 0)
		return (doexport(mini, buf2), free(buf2), 0);
	if (ft_strcmpspace("unset", buf2) == 0)
		return (dounset(mini, buf2), free(buf2), 0);
	if (ft_strcmpspace("env", buf2) == 0)
		return (print_env(mini->env), free(buf2), 0);
	if (ft_strcmpspace("$?", buf2) == 0)
		return (ft_putnbr_fd(g_status, 1), write(1, "\n", 1), free(buf2), 0);
	if (ft_strcmpspace("echo", buf2) == 0)
		return (buf2 = checkenvvars(buf2, mini), doecho(buf2), 0);
	if (ft_strchr(buf2, '=') && ft_strchr(buf2, '=')[-1] != ' '
		&& ft_strchr(buf2, '=')[1] != ' '
		&& ft_isgroup(ft_strchr(buf2, '=') + 1, ft_isbashprotected) == 0)
	{
		buf2 = checkenvvars(buf2, mini);
		entvars(&mini->env, ft_substr(buf2, 0,
				ft_strchr(buf2, '=') - buf2),
			ft_strdup(ft_strchr(buf2, '=') + 1));
		return (free(buf2), 0);
	}
	return (-1);
}
