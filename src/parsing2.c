/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing2.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jainavas <jainavas@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/20 19:00:12 by jainavas          #+#    #+#             */
/*   Updated: 2025/01/20 19:02:01 by jainavas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

void	split_input(char *buf, char ***cmd)
{
	char	*temp;
	int		i[3];

	i[0] = -1;
	i[1] = 0;
	i[2] = 0;
	temp = buf;
	while (buf[++i[0]])
	{
		if (i[1] == 0 && buf[i[0]] == '"')
			i[1] = 1;
		else if (i[1] == 0 && buf[i[0]] == '\'')
			i[1] = 2;
		else if ((i[1] == 1 && buf[i[0]] == '"')
			|| (i[1] == 2 && buf[i[0]] == '\''))
			i[1] = 0;
		if (i[1] == 0 && (buf[i[0]] == ' ' || buf[i[0] + 1] == '\0'))
		{
			(*cmd)[i[2]] = ft_strndup(temp, buf + i[0] + 1 - temp);
			i[2]++;
			temp = buf + i[0] + 1;
		}
	}
	(*cmd)[i[2]] = NULL;
}

int	count_params(char *buf)
{
	int	i[3];

	i[0] = -1;
	i[1] = 0;
	i[2] = 0;
	while (buf[++i[0]])
	{
		if (i[1] == 0 && buf[i[0]] == '"')
			i[1] = 1;
		else if (i[1] == 0 && buf[i[0]] == '\'')
			i[1] = 2;
		else if ((i[1] == 1 && buf[i[0]] == '"')
			|| (i[1] == 2 && buf[i[0]] == '\''))
			i[1] = 0;
		if (i[1] == 0 && (buf[i[0]] == ' ' || buf[i[0] + 1] == '\0'))
			i[2]++;
	}
	return (i[2]);
}

char	*process_vars(t_mini *mini, char *buf)
{
	int		i;
	int		mode;

	mode = 0;
	i = -1;
	while (buf[++i])
	{
		if (mode == 0 && buf[i] == '"')
			mode = 1;
		else if (mode == 0 && buf[i] == '\'')
			mode = 2;
		else if ((mode == 1 && buf[i] == '"') || (mode == 2 && buf[i] == '\''))
			mode = 0;
		else if (mode != 2 && buf[i] == '$')
			buf = expand_var(mini, buf, buf + i);
	}
	return (remove_quotes(buf));
}

char	*remove_quotes(char *buf)
{
	int		i;
	int		mode;

	mode = 0;
	i = -1;
	while (buf[++i])
	{
		if (mode == 0 && buf[i] == '"')
		{
			mode = 1;
			buf = replace_content(buf, NULL, buf + i--, 1);
		}
		else if (mode == 0 && buf[i] == '\'')
		{
			mode = 2;
			buf = replace_content(buf, NULL, buf + i--, 1);
		}
		else if ((mode == 1 && buf[i] == '"') || (mode == 2 && buf[i] == '\''))
		{
			mode = 0;
			buf = replace_content(buf, NULL, buf + i--, 1);
		}
	}
	return (buf);
}

char	*expand_var(t_mini *mini, char *str, char *name)
{
	int		len;
	t_env	*node;

	len = get_namelen(name);
	if (len > 1)
	{
		node = mini->env;
		while (node)
		{
			if (ft_strncmp(name + 1, node->name, len - 1) == 0)
			{
				str = replace_content(str, node->content, name, len);
				break ;
			}
			else if (node->next == NULL)
				str = replace_content(str, NULL, name, len);
			node = node->next;
		}
	}
	return (str);
}
