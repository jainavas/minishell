/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   environment1.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jainavas <jainavas@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/10 12:27:04 by mpenas-z          #+#    #+#             */
/*   Updated: 2025/01/08 18:07:04 by jainavas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/mini.h"

void	dpcheckenvars(char **buf, t_mini *mini)
{
	int	i;

	i = -1;
	while (buf[++i])
		buf[i] = checkenvvars(buf[i], mini);
}

char	*checkenvvars(char *buf, t_mini *mini)
{
	char	*tmp;

	if (mini->didcheckenv == 1 || !buf)
		return (buf);
	tmp = ft_strchr(buf, '$');
	if (!tmp)
		return (buf);
	tmp = checkenvlist(mini, &buf, tmp);
	if (tmp && ft_strchr(tmp + 1, '$'))
	{
		tmp = ft_strjoin_gnl(ft_substr(buf, 0, tmp - buf),
				ft_substr(ft_strchr(tmp + 1, '$'), 0,
					ft_strlen(ft_strchr(tmp + 1, '$'))));
		return (free(buf), mini->didcheckenv = 0, checkenvvars(tmp, mini));
	}
	else if (!tmp)
		return (mini->didcheckenv = 1, buf);
	tmp = ft_substr(buf, 0, tmp - buf);
	free(buf);
	mini->didcheckenv = 1;
	return (tmp);
}

char	*checkenvlist(t_mini *mini, char **buf, char *tmp)
{
	t_env	*var;
	int		i;
	
	var = mini->env;
	while (var)
	{
		tmp = checktmpslist(mini, buf, tmp);
		if (tmp && (ft_strcmpalnum(tmp + 1, var->name) == 0 
			|| (ft_strncmp(var->name, "?", 1) == 0 
				&& ft_strncmp(tmp + 1, "?", 1) == 0)))
		{
			i = (tmp - *buf) + ft_strlen(var->content);
			*buf = ft_strinsertdup(*buf, var->name, var->content, '$');
			var = mini->env;
			tmp = ft_strchr(&buf[0][i], '$');
		}
		var = var->next;
	}
	return (tmp);
}

t_env	*envarlast(t_env *lst)
{
	t_env	*tmp;

	tmp = lst;
	if (lst)
		while (tmp->next)
			tmp = tmp->next;
	return (tmp);
}

int	entvars(t_env **head, char *var, char *content)
{
	t_env	*new;

	if (get_env_var(head, var) != NULL)
		return (new = get_env_var(head, var), free(new->content),
			new->content = ft_strdup(content), free(var), free(content), 0);
	new = ft_calloc(1, sizeof(t_env));
	new->content = ft_strdup(content);
	if (ft_strncmp(var, "holatmp_", 8) == 0)
		var[8] = '0' + counttmps(*head);
	new->name = ft_strdup(var);
	new->next = NULL;
	new->prev = NULL;
	if (!*head)
		*head = new;
	else
	{
		(envarlast(*head))->next = new;
		new->prev = envarlast(*head);
	}
	return (free(content), free(var), 0);
}
