/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minish2.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jainavas <jainavas@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/10 15:56:12 by jainavas          #+#    #+#             */
/*   Updated: 2025/01/20 18:03:38 by jainavas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	recread(t_mini **mini)
{
	t_cmd	*head;
	char	**buf;
	char	*buf2;

	buf2 = readline("minishell% ");
	if (!buf2)
		return (1);
	if (buf2[0] == '\0')
		return (0);
	add_history(buf2);
	buf = cleannulls(process_input((*mini), ft_strdup(buf2)));
	head = evaluate_commands(buf);
	freedoublepointer(buf);
	if (!head)
		return (0);
	(*mini)->header = &head;
	head->oginput = ft_strdup(buf2);
	run_cmd_list(*mini, &head);
	free(buf2);
	free_cmd_list(&head);
	return (0);
}

int	recursiva(t_mini **mini)
{
	int		x;
	char	*temp;

	(*mini)->infile = NULL;
	(*mini)->didcheckenv = 0;
	x = recread(mini);
	while (x == 0)
	{
		temp = ft_itoa((*mini)->status);
		add_envar((*mini), "?", temp, 2);
		free (temp);
		freelist(*(*mini)->quotestmps);
		*(*mini)->quotestmps = NULL;
		(*mini)->infile = NULL;
		(*mini)->didcheckenv = 0;
		x = recread(mini);
	}
	return (x);
}
