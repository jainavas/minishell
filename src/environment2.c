/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   environment2.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jainavas <jainavas@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/10 12:40:23 by mpenas-z          #+#    #+#             */
/*   Updated: 2025/01/28 19:04:52 by jainavas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

t_env	*get_env_head(t_env *env)
{
	t_env	*aux;

	aux = env->prev;
	while (aux)
	{
		env = aux;
		aux = env->prev;
	}
	return (env);
}

t_env	*init_env_vars(char **envp)
{
	int		i;
	t_env	*head_env;
	t_env	*env;

	i = -1;
	head_env = NULL;
	while (envp[++i])
	{
		env = ft_calloc(1, sizeof(t_env));
		env->name = ft_strndup(envp[i], ft_strchr(envp[i], '=') - envp[i]);
		env->content = ft_strdup(ft_strchr(envp[i], '=') + 1);
		env->is_temp = 0;
		env->next = NULL;
		env->prev = head_env;
		if (head_env)
			head_env->next = env;
		head_env = env;
	}
	return (get_env_head(head_env));
}

t_env	*get_env_var(t_env	**head, char *varname)
{
	t_env	*aux_env;

	aux_env = *head;
	while (aux_env)
	{
		if (!ft_strcmpalnum(aux_env->name, varname))
			return (aux_env);
		aux_env = aux_env->next;
	}
	return (NULL);
}

void	add_temp_envar(t_mini *mini, char *varname)
{
	t_env	*aux_env;
	t_env	*env;

	if (exists_env_var(mini, varname))
		return ;
	aux_env = ft_calloc(1, sizeof(t_env));
	aux_env->name = ft_strdup(varname);
	aux_env->content = NULL;
	aux_env->is_temp = 1;
	aux_env->next = NULL;
	env = envarlast(mini->env);
	env->next = aux_env;
	aux_env->prev = env;
	return ;
}

void	add_envar(t_mini *mini, char *varname, char *value, int is_temp)
{
	t_env	*aux_env;
	t_env	*env;

	if (exists_env_var(mini, varname))
	{
		aux_env = get_env_var(&mini->env, varname);
		aux_env->is_temp = is_temp;
		if (aux_env->content)
			free (aux_env->content);
		aux_env->content = ft_strdup(value);
	}
	else
	{
		aux_env = ft_calloc(1, sizeof(t_env));
		aux_env->name = ft_strdup(varname);
		aux_env->content = ft_strdup(value);
		aux_env->is_temp = is_temp;
		aux_env->next = NULL;
		env = envarlast(mini->env);
		env->next = aux_env;
		if (env)
			aux_env->prev = env;
		else
			aux_env->prev = NULL;
	}
}
