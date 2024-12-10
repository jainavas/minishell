/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   environment2.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mpenas-z <mpenas-z@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/10 12:40:23 by mpenas-z          #+#    #+#             */
/*   Updated: 2024/12/10 13:53:08 by mpenas-z         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/mini.h"

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

void	add_temp_envar(char *varname, char *value)
{
	(void)varname;
	(void)value;
	return ;
}

void	add_envar(char *varname, char *value)
{
	(void)varname;
	(void)value;
	return;
}

void	print_temp_env(void)
{
	return ;
}
