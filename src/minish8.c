/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minish8.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jainavas <jainavas@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/02 21:16:21 by jainavas          #+#    #+#             */
/*   Updated: 2024/12/22 23:16:24 by mpenas-z         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini.h"

char	*putonlycmds(t_mini *mini, char *buf2, char *tmp)
{
	int	i;

	i = 1;
	while (tmp[i] == ' ')
		i++;
	while (ft_isalnum(tmp[i]) || tmp[i] == '.')
		i++;
	tmp = ft_substr(buf2, (tmp + 1) - buf2, i);
	mini->infile = ft_strtrim(tmp, " ");
	free(tmp);
	tmp = ft_substr(buf2, i, ft_strlen(buf2));
	free(buf2);
	buf2 = ft_strtrim(tmp, " ");
	free(tmp);
	return (buf2);
}

char	*simplequote(t_mini *mini, char *buf, char *tmp)
{
	char	*tmp2;

	tmp2 = ft_strchr(tmp + 1, 39);
	if (!tmp2)
		return (NULL);
	tmp = ft_substr(buf, tmp - buf + 1, tmp2 - tmp - 1);
	entvars(mini->quotestmps, ft_strdup("holatmp_0"), tmp);
	mini->quotesbuf = ft_strjoin_gnl(mini->quotesbuf, "$");
	tmp = ft_strtrim(envarlast(*mini->quotestmps)->name, " ");
	mini->quotesbuf = ft_strjoin_gnl(mini->quotesbuf, tmp);
	return (free(tmp), tmp2);
}

char	*doublequote(t_mini *mini, char *buf, char *tmp)
{
	char	*tmp2;

	tmp2 = ft_strchr(tmp + 1, '"');
	if (!tmp2)
		return (NULL);
	tmp = ft_substr(buf, tmp - buf + 1, tmp2 - tmp - 1);
	tmp = checkenvvars(tmp, mini);
	entvars(mini->quotestmps, ft_strdup("holatmp_0"), tmp);
	mini->quotesbuf = ft_strjoin_gnl(mini->quotesbuf, "$");
	tmp = ft_strtrim(envarlast(*mini->quotestmps)->name, " ");
	mini->quotesbuf = ft_strjoin_gnl(mini->quotesbuf, tmp);
	return (tmp);
}

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

	aux = pathseek(cmd, mini->envp);
	freedoublepointer(cmd);
	if (aux)
		return (free(aux), 0);
	else
		return (1);
}
