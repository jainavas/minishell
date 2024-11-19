/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mini.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jainavas <jainavas@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/09 02:16:09 by jainavas          #+#    #+#             */
/*   Updated: 2024/11/19 20:33:49 by jainavas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINI_H
# define MINI_H

# include "pipex.h"
# include <signal.h>
# include <readline/readline.h>
# include <readline/history.h>

typedef struct mini
{
	int		argc;
	char	**argv;
	char	**envp;
	int		out;
	char	*fileout;
	int		appendout;
	char	*infile;
	t_pipex	*pipex;
}	t_mini;

int		recread(t_mini *mini);
char	**preppipex(char *buf, char *infile, char *outfile, char **buf2);
int		alonecmdcall(int fdin, char **cmd, char *path, t_mini *mini);
void	anyfdtofile(int fd, char *filename, int out, int app);
int		checkkill(char *buf);
int		recursiva(t_mini *mini);
int		ft_dstrlen(char **s);
char	**preppipexlim(char *buf, char **antbuf);
void	docd(char *path);
int		pipex(int argc, char **argv, char **envp, t_mini *mini);
void	debuginout(char *buf2, char **buf, t_mini *mini);
char	**ft_split_cmds(char *str);
int		dolimitonecmd(char **buf, t_mini *mini);
int		dolimwithpipe(char *buf2, char **buf, t_mini *mini);
int		dopipes(char *buf2, char **buf, t_mini *mini);
int		docmd(char *buf2, char **buf, t_mini *mini);

#endif