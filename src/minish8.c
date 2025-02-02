/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minish8.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jainavas <jainavas@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/02 21:16:21 by jainavas          #+#    #+#             */
/*   Updated: 2025/02/02 13:34:10 by mpenas-z         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*checktmpslist(t_mini *mini, char **buf, char *tmp)
{
	t_env	*var;
	int		i;

	var = *mini->quotestmps;
	while (var)
	{
		if (tmp && ft_strcmpalnum(tmp + 1, var->name) == 0)
		{
			i = (tmp - *buf) + ft_strlen(var->content);
			*buf = ft_strinsertdup(*buf, var->name, var->content, '$');
			var = *mini->quotestmps;
			tmp = ft_strchr(&buf[0][i], '$');
			break ;
		}
		var = var->next;
	}
	return (tmp);
}

int	checkprepaths(char **cmd, t_mini *mini)
{
	char	*aux;

	aux = pathseekenv(cmd, mini->envp);
	freedoublepointer(cmd);
	if (aux)
		return (free(aux), 0);
	else
		return (1);
}

int	isthereanystdinnotreader(t_cmd *head)
{
	while (head)
	{
		if (!isstdinreader(head->cmd))
			return (1);
		head = head->next;
	}
	return (0);
}

int	isstdinreader(char *cmd)
{
	if (!ft_strncmp(cmd, "ls", 3) || !ft_strncmp(cmd, "pwd", 4)
		|| !ft_strncmp(cmd, "clear", 6) || !ft_strncmp(cmd, "env", 6))
		return (0);
	return (1);
}

void	check_void_aux(t_cmd *curr, char **aux)
{
	freedoublepointer(curr->argv);
	free (curr->cmd);
	if (aux[0])
	{
		curr->argv = aux;
		curr->cmd = ft_strdup(aux[0]);
	}
	else
	{
		freedoublepointer(aux);
		curr->argv = NULL;
		curr->cmd = NULL;
	}
}
