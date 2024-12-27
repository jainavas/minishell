/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mpenas-z <mpenas-z@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/24 18:22:58 by mpenas-z          #+#    #+#             */
/*   Updated: 2024/12/27 13:02:57 by mpenas-z         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/mini.h"

char	*process_input(t_mini *mini, char *buf)
{
	char	**cmd;
	char	*temp;
	char	*aux;
	int		param_count;
	int		i;

	aux = NULL;
	param_count = count_params(buf);
	cmd = ft_calloc(param_count + 1, sizeof(char *));
	split_input(buf, &cmd);
	free (buf);
	i = -1;
	while (cmd[++i])
	{
		cmd[i] = process_vars(mini, cmd[i]);
		temp = ft_strdup(cmd[i]);
		aux = ft_strjoin_gnl(aux, temp);
		free (temp);
	}
	freedoublepointer(cmd);
	return (aux);
}

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

char	*replace_content(char *str, char *content, char *start, int len)
{
	char	*temp;

	temp = ft_calloc(ft_strlen(str) - ft_strlen(start) + 1, sizeof(char));
	ft_strlcpy(temp, str, ft_strlen(str) - ft_strlen(start) + 1);
	if (content != NULL)
		temp = ft_strjoin_gnl(temp, content);
	if (start[len])
		temp = ft_strjoin_gnl(temp, start + len);
	free (str);
	return (temp);
}

int	get_namelen(char *name)
{
	int	i;

	if (!name) 
		return (0);
	if (name[1] == '?')
		return (2);
	i = 1;
	while (name[i] && (isalnum(name[i]) || name[i] == '_'))
			i++;
	return (i);
}
