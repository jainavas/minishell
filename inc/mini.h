/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mini.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jainavas <jainavas@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/09 02:16:09 by jainavas          #+#    #+#             */
/*   Updated: 2024/12/08 15:14:59 by mpenas-z         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINI_H
# define MINI_H

# include "../libft_ext/libft.h"
# include "../libft_ext/ft_printf.h"
# include <signal.h>
# include <stdio.h>
# include <readline/readline.h>
# include <readline/history.h>
# include "pipex.h"

extern int	g_status;

typedef struct sigaction	t_sig;

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
	char	*infile;
	char	*quotesbuf;
	t_envar	**envars;
}	t_mini;

/* ft_splitchats.c */
char	**ft_splitchars(char *str, char *charset);
/* minish.c */
char	**preppipex(char *buf, char *infile, char **buf2);
int		alonecmdcall(int fdin, char **cmd, char *path, t_mini *mini);
void	anyfdtofile(int fd, char *filename, int app);
int		checkkill(char *buf);
/* minish2.c */
int		recread(t_mini *mini);
int		checkinfile(t_mini *mini);
int		recursiva(t_mini *mini);
char	**preppipexlim(char *buf, char **antbuf);
/* minish3.c */
void	docd(char *path);
int		ft_dstrchr(char **s, char *s2);
int		ft_dstrlen(char **s);
char	*debuginout(char *buf2, t_mini *mini);
int		dolimitonecmd(char **buf, t_mini *mini);
/* minish4.c */
int		dolimwithpipe(char *buf2, char **buf, t_mini *mini);
int		dopipes(char *buf2, char **buf, t_mini *mini);
int		docmd(char *buf2, char **buf, t_mini *mini);
int		checkquotes(char *buf, t_mini *mini);
/* minish5.c */
void	doecho(char *buf);
void	entvars(t_envar **head, char *var, char *content);
char	*checkenvvars(char *buf, t_mini *mini);
void	freelist(t_envar **lst);
void	newfileout(t_fout **head, char *file, int app);
/* minish6.c */
t_fout	*foutlast(t_fout *lst);
t_envar	*envarlast(t_envar *lst);
void	handlemfilesout(t_mini *mini, char *buf);
void	fdtomfiles(t_mini *mini, int fd);
void	freeoutfiles(t_fout **lst);
/* minish7.c */
int		counttmps(t_envar **lst);
void	dpcheckenvars(char **buf, t_mini *mini);
int		builtins(t_mini *mini, char *buf2);
char	*initialdebug(t_mini *mini, char *buf2);
int		exec(t_mini *mini, char *buf2, char **buf);
/* minish8.c */
char	*putonlycmds(t_mini *mini, char *buf2, char *tmp);
char	*checkenvlist(t_mini *mini, char **buf, char *tmp);
char	*simplequote(t_mini *mini, char *buf, char *tmp);
char	*doublequote(t_mini *mini, char *buf, char *tmp);
/* signals.c */
void	set_signals(void);
void	handle_sigint(int sig);
/* pipex.c */
int		pipex(int argc, char **argv, char **envp, t_mini *mini);

#endif
