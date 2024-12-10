/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   environment1.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mpenas-z <mpenas-z@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/10 12:27:04 by mpenas-z          #+#    #+#             */
/*   Updated: 2024/12/10 12:40:47 by mpenas-z         ###   ########.fr       */
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

	tmp = ft_strchr(buf, '$');
	if (!tmp)
		return (buf);
	tmp = checkenvlist(mini, &buf, tmp);
	if (tmp && ft_strchr(tmp + 1, '$'))
	{
		tmp = ft_strjoin_gnl(ft_substr(buf, 0, tmp - buf),
				ft_substr(ft_strchr(tmp + 1, '$'), 0,
					ft_strlen(ft_strchr(tmp + 1, '$'))));
		return (free(buf), checkenvvars(tmp, mini));
	}
	else if (!tmp)
		return (buf);
	tmp = ft_substr(buf, 0, tmp - buf);
	free(buf);
	return (tmp);
}

char	*checkenvlist(t_mini *mini, char **buf, char *tmp)
{
	t_envar	*var;
	int		i;

	var = *(mini->envars);
	while (var)
	{
		if (tmp && ft_strncmp(tmp + 1, var->name, ft_strlen(var->name) - 1) == 0
			&& ft_isalnum(tmp[ft_strlen(var->name)]) == 0)
		{
			i = (tmp - *buf) + ft_strlen(var->content);
			*buf = ft_strinsertdup(*buf, var->name, var->content);
			var = *(mini->envars);
			tmp = ft_strchr(&buf[0][i], '$');
		}
		var = var->next;
	}
	return (tmp);
}

t_envar	*envarlast(t_envar *lst)
{
	t_envar	*tmp;

	tmp = lst;
	if (lst)
		while (tmp->next)
			tmp = tmp->next;
	return (tmp);
}

void	entvars(t_envar **head, char *var, char *content)
{
	t_envar	*new;

	new = ft_calloc(1, sizeof(t_envar));
	new->content = ft_strdup(content);
	if (ft_strncmp(var, "holatmp_", 8) == 0)
		var[8] = '0' + counttmps(head);
	new->name = malloc(ft_strlen(var) + 2);
	ft_strlcpy(new->name, var, ft_strlen(var) + 1);
	ft_strlcat(new->name, " ", ft_strlen(var) + 2);
	new->next = NULL;
	new->prev = NULL;
	if (!*head)
		*head = new;
	else
	{
		(envarlast(*head))->next = new;
		new->prev = envarlast(*head);
	}
	free(content);
	free(var);
}
