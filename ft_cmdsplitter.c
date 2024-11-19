/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cmdsplitter.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jainavas <jainavas@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/16 19:23:59 by jainavas          #+#    #+#             */
/*   Updated: 2024/11/19 20:27:02 by jainavas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini.h"

static int	count_cmds(char *str)
{
	int	i;
	int	words;

	words = 0;
	i = 0;
	while (str[i] != '\0')
	{
		if (((ft_isalnum(str[i + 1]) == 0 && str[i + 1] != ' '
					&& str[i + 1] != '-') || str[i + 1] == '\0')
			&& !((ft_isalnum(str[i]) == 0 && str[i] != ' ' && str[i] != '-')
				|| str[i] == '\0'))
			words++;
		i++;
	}
	return (words);
}

static void	write_cmd(char *dest, char *from, int j)
{
	int		i;

	i = 0;
	while (i < j)
	{
		dest[i] = from[i];
		i++;
	}
	dest[i] = '\0';
}

static int	write_splitcmd(char **split, char *str)
{
	int		i;
	int		j;
	int		word;

	word = 0;
	i = 0;
	while (str[i] != '\0')
	{
		if ((ft_isalnum(str[i]) == 0 && str[i] != ' '
				&& str[i] != '-') || str[i] == '\0')
			i++;
		else
		{
			j = 0;
			while (!((ft_isalnum(str[i + j]) == 0 && str[i + j] != ' '
						&& str[i + j] != '-') || str[i + j] == '\0'))
				j++;
			split[word] = (char *)malloc(sizeof(char) * (j + 1));
			if (split[word] == NULL)
				return (1);
			write_cmd(split[word++], str + i, j);
			i += j;
		}
	}
	return (0);
}

static void	protectsplit(char **split)
{
	int	i;

	i = 0;
	while (split[i])
		free(split[i++]);
	free(split);
}

char	**ft_split_cmds(char *str)
{
	char	**split;
	char	*tmp;
	int		words;

	if (!str)
		return (NULL);
	words = count_cmds(str);
	split = (char **)malloc(sizeof(char *) * (words + 1));
	if (split == NULL)
		return (NULL);
	split[words] = 0;
	if (write_splitcmd(split, str) == 1)
		protectsplit(split);
	words = -1;
	while (split[++words])
	{
		tmp = ft_strtrim(split[words], " ");
		free(split[words]);
		split[words] = tmp;
	}
	return (split);
}

// int main(void)
// {
// 	int	i = -1;
// 	char **buf = ft_split_cmds("grep a << lim | wc");
// 	while(buf[++i])
// 	{
// 		printf("|%s|", buf[i]);
// 		free(buf[i]);
// 	}
// 	free(buf);
// }