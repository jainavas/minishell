/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minish7.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jainavas <jainavas@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/30 16:38:08 by jainavas          #+#    #+#             */
/*   Updated: 2024/12/06 18:43:50 by mpenas-z         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini.h"

int	counttmps(t_envar **lst)
{
	t_envar	*new;
	int		i;

	new = *lst;
	i = 0;
	while (new != NULL)
	{
		if (strncmp("holatmp_", new->name, 8) == 0)
			i++;
		new = new->next;
	}
	return (i);
}

void	dpcheckenvars(char **buf, t_mini *mini)
{
	int	i;

	i = -1;
	while (buf[++i])
		buf[i] = checkenvvars(buf[i], mini);
}

int	builtins(t_mini *mini, char *buf2)
{
	if (checkkill(buf2))
		return (free(buf2), rl_clear_history(), 1);
	if (ft_strncmp("cd ", buf2, 3) == 0)
		return (buf2 = checkenvvars(buf2, mini), docd(&buf2[3]), free(buf2), 0);
	if (ft_strncmp("echo ", buf2, 5) == 0)
		return (buf2 = checkenvvars(buf2, mini), doecho(buf2), 0);
	if (ft_strchr(buf2, '=') && ft_strchr(buf2, '=')[-1] != ' '
		&& ft_strchr(buf2, '=')[1] != ' ')
	{
		buf2 = checkenvvars(buf2, mini);
		entvars(mini->envars, ft_substr(buf2, 0,
				ft_strchr(buf2, '=') - buf2),
			ft_strdup(ft_strchr(buf2, '=') + 1));
		return (free(buf2), 0);
	}
	return (-1);
}

char	*initialdebug(t_mini *mini, char *buf2)
{
	if (!buf2)
		return (NULL);
	if (buf2[0] == '\0')
		return (free(buf2), NULL);
	add_history(buf2);
	if (ft_strchrtwo(buf2, '"', 39))
	{
		if (checkquotes(buf2, mini) == -1)
			return (write(1, "Error\n", 6), free(buf2),
				free(mini->quotesbuf), mini->quotesbuf = NULL, NULL);
		free(buf2);
		buf2 = ft_strdup(mini->quotesbuf);
		free(mini->quotesbuf);
		mini->quotesbuf = NULL;
		return (buf2);
	}
	else
		return (checkenvvars(buf2, mini));
}

int	exec(t_mini *mini, char *buf2, char **buf)
{
	if ((ft_strchr(buf2, '<') != NULL && ft_strchr(buf2, '<')[1] == '<') &&
		ft_strchr(buf2, '|') != NULL)
		return (dolimwithpipe(buf2, buf, mini));
	else if (ft_strchr(buf2, '|') != NULL)
		return (dopipes(buf2, buf, mini));
	else
		return (docmd(buf2, buf, mini));
}
