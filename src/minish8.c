/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minish8.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jainavas <jainavas@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/02 21:16:21 by jainavas          #+#    #+#             */
/*   Updated: 2025/01/20 18:11:05 by jainavas         ###   ########.fr       */
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
