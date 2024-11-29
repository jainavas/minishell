/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minish5.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jainavas <jainavas@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/21 22:28:59 by jainavas          #+#    #+#             */
/*   Updated: 2024/11/28 18:47:17 by jainavas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini.h"

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

void	entvars(t_envar **head, char *var, char *content)
{
	t_envar	*new;

	new = ft_calloc(1, sizeof(t_envar));
	new->content = ft_strdup(content);
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

char	*checkenvvars(char *buf, t_mini *mini)
{
	t_envar	*var;
	char	*tmp;

	var = *(mini->envars);
	tmp = NULL;
	tmp = ft_strchr(buf, '$');
	if (var && tmp)
	{
		while (var)
		{
			if (ft_strncmp(tmp + 1, var->name, ft_strlen(var->name) - 1) == 0 && ft_isalnum(tmp[ft_strlen(var->name)]) == 0 )
			{
				buf = ft_strinsertdup(buf, var->name, var->content);
				return (checkenvvars(buf, mini));
			}
			var = var->next;
		}
		if (ft_strchr(tmp + 1, '$'))
		{
			tmp = ft_strjoin_gnl(ft_substr(buf, 0, tmp - buf), ft_substr(ft_strchr(tmp + 1, '$'), 0, ft_strlen(ft_strchr(tmp + 1, '$'))));
			return (free(buf), checkenvvars(tmp, mini));
		}
		else
			tmp = ft_substr(buf, 0, tmp - buf);
		free(buf);
		return (tmp);
	}
	return (buf);
}

void	freelist(t_envar **lst)
{
	t_envar	*new;

	while (*lst != NULL)
	{
		new = *lst;
		*lst = new->next;
		free(new->content);
		free(new->name);
		free(new);
	}
}

void	newfileout(t_fout **head, char *file, int app)
{
	t_fout	*new;

	new = ft_calloc(1, sizeof(t_fout));
	new->file = ft_strtrim(file, " >");
	new->out = 1;
	new->appendout = app;
	new->next = NULL;
	new->prev = NULL;
	if (!*head)
		*head = new;
	else
	{
		(foutlast(*head))->next = new;
		new->prev = foutlast(*head);
	}
	free(file);
}
