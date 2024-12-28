/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mpenas-z <mpenas-z@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/24 18:22:58 by mpenas-z          #+#    #+#             */
/*   Updated: 2024/12/28 18:32:49 by mpenas-z         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/mini.h"

char	**process_input(t_mini *mini, char *buf)
{
	char	**cmd;
	char	*temp;
	int		param_count;
	int		i;

	param_count = count_params(buf);
	cmd = ft_calloc(param_count + 1, sizeof(char *));
	split_input(buf, &cmd);
	free (buf);
	i = -1;
	while (cmd[++i])
	{
		temp = cmd[i];
		cmd[i] = ft_strtrim(cmd[i], " ");
		free (temp);
	}
	process_operators(&cmd);
	i = -1;
	while (cmd[++i])
		cmd[i] = process_vars(mini, cmd[i]);
	return (cmd);
}

int	count_operators(char *buf)
{
	int	i[3];

	i[2] = 0;
	i[1] = 0;
	i[0] = -1;
	while ((size_t)++i[0] <= ft_strlen(buf))
	{
		if (i[1] == 0 && buf[i[0]] == '"')
			i[1] = 1;
		else if (i[1] == 0 && buf[i[0]] == '\'')
			i[1] = 2;
		else if ((i[1] == 1 && buf[i[0]] == '"')
			|| (i[1] == 2 && buf[i[0]] == '\''))
			i[1] = 0;
		else if (i[1] == 0 && (buf[i[0]] == '<' || buf[i[0]] == '>'
				|| buf[i[0]] == '|' || buf[i[0]] == '\0'))
		{
			if (i[0] > 0 && buf[i[0] - 1] != '<' && buf[i[0] - 1] != '>'
				&& buf[i[0] - 1] != '|' && buf[i[0] - 1] != '\0')
				i[2]++;
			if (buf[i[0]] != '\0')
				i[2]++;
		}
	}
	return (i[2]);
}

void	is_in_quotes(int *mode, char c)
{
	if (*mode == 0 && c == '"')
		*mode = 1;
	else if (*mode == 0 && c == '\'')
		*mode = 2;
	else if ((*mode == 1 && c == '"') || (*mode == 2 && c == '\''))
		*mode = 0;
}

char	**split_operators(int count, char *buf)
{
	char	**split;
	char	*temp;
	int		i[3];

	split = ft_calloc(count + 1, sizeof(char *));
	temp = buf;
	i[2] = 0;
	i[1] = 0;
	i[0] = -1;
	while ((size_t)++i[0] <= ft_strlen(buf))
	{
		is_in_quotes(&i[1], buf[i[0]]);
		if (i[1] == 0 && (buf[i[0]] == '<' || buf[i[0]] == '>'
				|| buf[i[0]] == '|' || buf[i[0]] == '\0'))
		{
			if (temp < buf + i[0])
				split[i[2]++] = ft_strndup(temp, buf + i[0] - temp);
			if (buf[i[0]] != '\0')
				split[i[2]++] = ft_strndup(buf + i[0], 1);
			temp = buf + i[0] + 1;
		}
	}
	return (split);
}

void	process_operators(char ***cmd)
{
	char	**input;
	char	**split;
	int		i[3];

	input = (*cmd);
	i[0] = -1;
	i[1] = 0;
	while (input[++i[0]])
		i[1] += count_operators(input[i[0]]);
	(*cmd) = ft_calloc(i[1] + 1, sizeof(char *));
	i[2] = -1;
	i[0] = -1;
	while (input[++i[0]])
	{
		i[1] = -1;
		split = split_operators(count_operators(input[i[0]]), input[i[0]]);
		while (split[++i[1]])
			(*cmd)[++i[2]] = split[i[1]];
		free(split);
		free(input[i[0]]);
	}
	free(input);
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
