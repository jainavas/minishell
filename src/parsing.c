/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jainavas <jainavas@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/24 18:22:58 by mpenas-z          #+#    #+#             */
/*   Updated: 2025/01/28 15:27:42 by mpenas-z         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

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
		is_in_quotes(&i[1], buf[i[0]]);
		if (i[1] == 0 && (buf[i[0]] == '<' || buf[i[0]] == '>'
				|| buf[i[0]] == '|' || buf[i[0]] == '\0'))
			increment_operators(&i[2], &i[0], buf);
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
				cases_split_operators(&i[0], &i[2], buf, split);
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
