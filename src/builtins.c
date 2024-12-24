/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jainavas <jainavas@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/10 12:14:17 by mpenas-z          #+#    #+#             */
/*   Updated: 2024/12/24 15:51:44 by mpenas-z         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/mini.h"

int	docd(char *path, t_mini *mini)
{
	t_env	*tmp;
	char	*str;

	if (path[spacesindex(path + 2) + 2] == '\0')
	{
		tmp = get_env_var(&mini->env, "HOME");
		chdirandoldpwd(tmp->content, mini);
		return (free(path), 0);
	}
	else if (path[spacesindex(path + 2) + 2] == '~')
	{
		tmp = get_env_var(&mini->env, "HOME");
		str = ft_strinsertdup(strdup(path + spacesindex(path + 2) + 2), "",
				tmp->content, '~');
		chdirandoldpwd(str, mini);
		return (free(str), free(path), 0);
	}
	else if (access(&path[spacesindex(path + 2) + 2], F_OK) == 0)
		return (chdirandoldpwd(&path[spacesindex(path + 2) + 2], mini),
			free(path), 0);
	else
		return (ft_printf("cd: no such file or directory: %s\n", &path[3]),
			free(path), 0);
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

int	doexport(t_mini *mini, char *buf)
{
	int		argc;
	int		status;
	char	**args;
	char	**parsed_line;

	args = ft_split(buf, ' ');
	argc = 0;
	status = 0;
	while (args[++argc])
	{
		if (!is_valid_identifier(args[argc]))
		{
			ft_putstr_fd("export: not a valid identifier\n", 2);
			status = 1;
		}
		else
		{
			parsed_line = ft_split(args[argc], '=');
			if (!parsed_line[1])
				add_temp_envar(mini, parsed_line[0]);
			else
				add_envar(mini, parsed_line[0], parsed_line[1], 1);
			freedoublepointer(parsed_line);
		}
	}
	if (argc == 1)
		print_temp_env(mini->env);
	freedoublepointer(args);
	return (status);
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
		return ;
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
	int	status;

	status = 0;
	if (checkkill(buf2))
		return (status = -1, free(buf2), rl_clear_history(), status);
	if (ft_strcmpspace("cd", buf2) == 0)
		return (status = docd(checkenvvars(buf2, mini), mini), status);
	if (ft_strcmpspace("export", buf2) == 0)
		return (status = doexport(mini, buf2), free(buf2), status);
	if (ft_strcmpspace("unset", buf2) == 0)
		return (dounset(mini, buf2), free(buf2), status);
	if (ft_strcmpspace("env", buf2) == 0)
		return (print_env(mini->env), free(buf2), status);
	if (ft_strcmpspace("echo", buf2) == 0)
		return (buf2 = checkenvvars(buf2, mini), doecho(buf2), status);
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
	return (-2);
}
