/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mini.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jainavas <jainavas@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/09 02:16:09 by jainavas          #+#    #+#             */
/*   Updated: 2024/11/30 20:21:42 by jainavas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINI_H
# define MINI_H

# include <signal.h>
# include <readline/readline.h>
# include <readline/history.h>
# include "pipex.h"

typedef struct envvar
{
	char			*name;
	char			*content;
	struct envvar	*next;
	struct envvar	*prev;
}	t_envar;

typedef struct fileout
{
	char			*file;
	int				out;
	int				appendout;
	struct fileout	*next;
	struct fileout	*prev;
}	t_fout;

typedef struct mini
{
	int		argc;
	char	**argv;
	char	**envp;
	t_fout	**mfilesout;
	int		out;
	char	*fileout;
	int		appendout;
	char	*infile;
	char	*quotesbuf;
	t_envar	**envars;
}	t_mini;

int		recread(t_mini *mini);
char	**preppipex(char *buf, char *infile, char **buf2);
int		alonecmdcall(int fdin, char **cmd, char *path, t_mini *mini);
void	anyfdtofile(int fd, char *filename, int app);
int		checkkill(char *buf);
int		recursiva(t_mini *mini);
int		ft_dstrlen(char **s);
char	**preppipexlim(char *buf, char **antbuf);
void	docd(char *path);
int		pipex(int argc, char **argv, char **envp, t_mini *mini);
char	*debuginout(char *buf2, t_mini *mini);
char	**ft_splitchars(char *str, char *charset);
int		dolimitonecmd(char **buf, t_mini *mini);
int		dolimwithpipe(char *buf2, char **buf, t_mini *mini);
int		dopipes(char *buf2, char **buf, t_mini *mini);
int		docmd(char *buf2, char **buf, t_mini *mini);
void	doecho(char *buf);
void	entvars(t_envar **head, char *var, char *content);
char	*checkenvvars(char *buf, t_mini *mini);
void	freelist(t_envar **lst);
int		checkquotes(char *buf, t_mini *mini);
void	newfileout(t_fout **head, char *file, int app);
void	fdtomfiles(t_mini *mini, int fd);
void	handlemfilesout(t_mini *mini, char *buf);
void	freeoutfiles(t_fout **lst);
t_envar	*envarlast(t_envar *lst);
t_fout	*foutlast(t_fout *lst);
int		counttmps(t_envar **lst);
void	dpcheckenvars(char **buf, t_mini *mini);

#endif