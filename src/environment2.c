/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   environment2.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jainavas <jainavas@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/10 12:40:23 by mpenas-z          #+#    #+#             */
/*   Updated: 2024/12/16 18:17:43 by jainavas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/mini.h"

void	freelist(t_env *lst)
{
	t_env	*new;

	while (lst != NULL)
	{
		new = lst;
		lst = new->next;
		free(new->content);
		free(new->name);
		free(new);
	}
}

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
	char	**aux_buffs;

	i = -1;
	head_env = NULL;
	while (envp[++i])
	{
		env = ft_calloc(1, sizeof(t_env));
		aux_buffs = ft_split(envp[i], '=');
		env->name = ft_strdup(aux_buffs[0]);
		env->content = ft_strdup(aux_buffs[1]);
		env->is_temp = 0;
		env->next = NULL;
		env->prev = head_env;
		if (head_env)
			head_env->next = env;
		head_env = env;
		freedoublepointer(aux_buffs);
	}
	return (get_env_head(head_env));
}

int	exists_env_var(t_mini *mini, char *varname)
{
	t_env	*aux_env;

	aux_env = mini->env;
	while (aux_env)
	{
		if (!ft_strncmp(aux_env->name, varname, ft_strlen(varname)))
			return (1);
		aux_env = aux_env->next;
	}
	return (0);
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
	aux_env->content = ft_strdup("''");
	aux_env->is_temp = 1;
	aux_env->next = NULL;
	env = envarlast(mini->env);
	env->next = aux_env;
	aux_env->prev = env;
	return ;
}

void	add_envar(t_mini *mini, char *varname, char *value)
{
	t_env	*aux_env;
	t_env	*env;

	if (exists_env_var(mini, varname))
	{
		aux_env = get_env_var(&mini->env, varname);
		aux_env->is_temp = 0;
		if (aux_env->content)
			free (aux_env->content);
		aux_env->content = ft_strdup(value);
	}
	else
	{
		aux_env = ft_calloc(1, sizeof(t_env));
		aux_env->name = ft_strdup(varname);
		aux_env->content = ft_strdup(value);
		aux_env->is_temp = 0;
		aux_env->next = NULL;
		env = envarlast(mini->env);
		env->next = aux_env;
		aux_env->prev = env;
	}
}

void	remove_envar(t_mini *mini, char *varname)
{
	t_env	*aux_env;
	t_env	*env;

	if (exists_env_var(mini, varname))
	{
		aux_env = get_env_var(&mini->env, varname);
		env = aux_env->prev;
		env->next = aux_env->next;
		if (aux_env->next)
			aux_env->next->prev = env;
		if (aux_env->content)
			free (aux_env->content);
		if (aux_env->name)
			free (aux_env->name);
		free (aux_env);
	}
	(void)aux_env;
	(void)env;
}

void	print_temp_env(t_env *env)
{
	t_env	*aux;
	
	aux = env;
	while (aux)
	{
		env = aux;
		printf("%s=%s\n", env->name, env->content);
		aux = env->next;
	}
	return ;
}

void	print_env(t_env *env)
{
	t_env	*aux;
	
	aux = env;
	while (aux)
	{
		env = aux;
		if (env->is_temp != 1)
			printf("%s=%s\n", env->name, env->content);
		aux = env->next;
	}
	return ;
}
