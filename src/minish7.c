/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minish7.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jainavas <jainavas@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/30 16:38:08 by jainavas          #+#    #+#             */
/*   Updated: 2024/12/22 18:56:25 by mpenas-z         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini.h"

int	counttmps(t_env *lst)
{
	t_env	*new;
	int		i;

	new = lst;
	i = 0;
	while (new != NULL)
	{
		if (strncmp("holatmp_", new->name, 8) == 0)
			i++;
		new = new->next;
	}
	return (i);
}

char	*initialdebug(t_mini *mini, char *buf2)
{
	char	*tmp;

	if (!buf2)
		return (NULL);
	if (buf2[0] == '\0')
		return (free(buf2), NULL);
	tmp = ft_strtrim(buf2, " ");
	free(buf2);
	buf2 = tmp;
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
	else if (ft_strncmp(buf2, "$?", 2) != 0)
		return (checkenvvars(buf2, mini));
	else
		return (buf2);
}

int	exec(t_mini *mini, char *buf2, char **buf)
{
	g_status = 0;
	if ((ft_strchr(buf2, '<') != NULL && ft_strchr(buf2, '<')[1] == '<') &&
		ft_strchr(buf2, '|') != NULL)
		return (dolimwithpipe(buf2, buf, mini));
	else if (ft_strchr(buf2, '|') != NULL)
		return (dopipes(buf2, buf, mini));
	else
		return (docmd(buf2, buf, mini));
}
