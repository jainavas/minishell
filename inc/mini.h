/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mini.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jainavas <jainavas@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/09 02:16:09 by jainavas          #+#    #+#             */
/*   Updated: 2024/12/22 20:40:45 by mpenas-z         ###   ########.fr       */
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

extern int					g_status;

typedef struct sigaction	t_sig;

typedef struct s_env
{
	char					*name;
	char					*content;
	int						is_temp;
	struct s_env			*next;
	struct s_env			*prev;
}	t_env;

typedef struct s_fileout
{
	char					*file;
	int						out;
	int						appendout;
	struct s_fileout		*next;
	struct s_fileout		*prev;
}	t_fout;

typedef struct mini
{
	int						argc;
	char					**argv;
	char					**envp;
	t_fout					**mfilesout;
	char					*infile;
	char					*quotesbuf;
	int						didcheckenv;
	t_env					*env;
	t_env					**quotestmps;
}	t_mini;

/* ft_splitchats.c */
char	**ft_splitchars(char *str, char *charset);
/* minish.c */
char	**preppipex(char *buf, char *infile, char **buf2, t_mini *mini);
int		alonecmdcall(int fdin, char **cmd, char *path, t_mini *mini);
void	anyfdtofile(int fd, char *filename, int app);
int		checkkill(char *buf);
/* minish2.c */
int		recread(t_mini **mini);
int		checkinfile(t_mini *mini);
int		recursiva(t_mini **mini);
char	**preppipexlim(char *buf, char **antbuf, t_mini *mini);
/* minish3.c */
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
void	newfileout(t_fout **head, char *file, int app);
void	alonecmdcallutils(int fd[2], int fdin);
int		spacesindex(const char *str);
/* minish6.c */
t_fout	*foutlast(t_fout *lst);
void	handlemfilesout(t_mini *mini, char *buf);
void	fdtomfiles(t_mini *mini, int fd);
void	freeoutfiles(t_fout **lst);
/* minish7.c */
int		counttmps(t_env *lst);
char	*initialdebug(t_mini *mini, char *buf2);
int		exec(t_mini *mini, char *buf2, char **buf);
/* minish8.c */
char	*putonlycmds(t_mini *mini, char *buf2, char *tmp);
char	*simplequote(t_mini *mini, char *buf, char *tmp);
char	*doublequote(t_mini *mini, char *buf, char *tmp);
char	*checktmpslist(t_mini *mini, char **buf, char *tmp);
int		checkprepaths(char **cmd, t_mini *mini);
/* signals.c */
void	set_signals(void);
void	handle_sigint(int sig);
void	new_prompt(void);
/* builtins.c */
int		docd(char *path, t_mini *mini);
void	doecho(char *buf);
void	doexport(t_mini *mini, char *buf);
void	dounset(t_mini *mini, char *buf);
int		builtins(t_mini *minish, char *buf2);
/* builtins2.c */
int		is_bad_assignment(char *buf);
void	chdirandoldpwd(char *new, t_mini *mini);
/* environment1.c */
void	dpcheckenvars(char **buf, t_mini *mini);
t_env	*envarlast(t_env *lst);
char	*checkenvlist(t_mini *mini, char **buf, char *tmp);
int		entvars(t_env **head, char *var, char *content);
char	*checkenvvars(char *buf, t_mini *mini);
/* environment2.c */
void	freelist(t_env *lst);
t_env	*get_env_head(t_env *env);
t_env	*get_env_var(t_env **head, char *varname);
t_env	*init_env_vars(char **envp);
void	add_temp_envar(t_mini *mini, char *varname);
void	add_envar(t_mini *mini, char *varname, char *value);
void	remove_envar(t_mini *mini, char *varname);
void	print_temp_env(t_env *env);
void	print_env(t_env *env);
/* pipex.c */
int		pipex(int argc, char **argv, char **envp, t_mini *mini);

#endif
