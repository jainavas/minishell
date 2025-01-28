/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   environment3.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jainavas <jainavas@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/20 18:45:28 by jainavas          #+#    #+#             */
/*   Updated: 2025/01/28 20:12:56 by mpenas-z         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

void	remove_envar(t_mini *mini, char *varname)
{
	t_env	*aux_env;
	t_env	*env;

	if (exists_env_var(mini, varname))
	{
		aux_env = get_env_var(&mini->env, varname);
		env = aux_env->prev;
		if (env)
			env->next = aux_env->next;
		else
			mini->env = aux_env->next;
		if (aux_env->next)
			aux_env->next->prev = env;
		if (aux_env->content)
			free (aux_env->content);
		if (aux_env->name)
			free (aux_env->name);
		free (aux_env);
	}
}

void	print_temp_env(t_env *env, int fd)
{
	t_env	*aux;

	aux = env;
	while (aux)
	{
		env = aux;
		if (env->is_temp != 2 && env->content)
		{
			ft_putstr_fd("declare -x ", fd);
			ft_putstr_fd(env->name, fd);
			ft_putstr_fd("=\"", fd);
			ft_putstr_fd(env->content, fd);
			ft_putstr_fd("\"\n", fd);
		}
		else if (env->is_temp != 2 && !env->content)
		{
			ft_putstr_fd("declare -x ", fd);
			ft_putendl_fd(env->name, fd);
		}
		aux = env->next;
	}
	if (fd > 2 && fd != -1)
		close(fd);
}

void	print_envfd(t_env *env, int fd)
{
	t_env	*aux;

	aux = env;
	while (aux && fd != -1)
	{
		env = aux;
		if (env->is_temp == 0 && env->name && env->content)
		{
			ft_putstr_fd(env->name, fd);
			ft_putstr_fd("=", fd);
			ft_putendl_fd(env->content, fd);
		}
		aux = env->next;
	}
	if (fd > 2 && fd != -1)
		close(fd);
}

int	envsize(t_env *env)
{
	t_env	*aux;
	int		res;

	aux = env;
	res = 0;
	while (aux)
	{
		if (env->is_temp == 0)
			res++;
		aux = aux->next;
	}
	return (res);
}

char	**envtodoublechar(t_env *env)
{
	int		tmpfd;
	int		i;
	char	*tmp;
	char	**res;

	i = -1;
	tmpfd = open("tmpenv", O_CREAT, O_RDWR);
	print_envfd(env, tmpfd);
	tmpfd = open("tmpenv", O_RDWR);
	tmp = get_next_line(tmpfd);
	res = ft_calloc(envsize(env) + 1, sizeof(char *));
	while (tmp)
	{
		res[++i] = tmp;
		printf("%s", res[i]);
		tmp = get_next_line(tmpfd);
	}
	res[++i] = NULL;
	if (tmpfd != -1)
		close(tmpfd);
	tmp = ft_strjoin_gnl(getcwd(NULL, 0), "/tmpenv");
	unlink(tmp);
	free(tmp);
	return (res);
}
