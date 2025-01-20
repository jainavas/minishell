/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jainavas <jainavas@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/09 02:16:09 by jainavas          #+#    #+#             */
/*   Updated: 2025/01/20 23:08:42 by jainavas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H
# define READ_FD 0
# define WRITE_FD 1

# include "../libft_ext/libft.h"
# include "../libft_ext/ft_printf.h"
# include <signal.h>
# include <stdio.h>
# include <fcntl.h>
# include <stdlib.h>
# include <string.h>
# include <unistd.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <sys/stat.h>
# include <dirent.h>

extern int					g_status;

typedef struct sigaction	t_sig;

typedef struct s_fileout
{
	char					*file;
	int						foutn;
	int						out;
	int						priorinfout;
	int						appendout;
	struct s_fileout		*next;
	struct s_fileout		*prev;
}	t_fout;

typedef struct s_env
{
	char					*name;
	char					*content;
	int						is_temp;
	struct s_env			*next;
	struct s_env			*prev;
}	t_env;

typedef struct s_filefinder
{
	DIR				*dir;
	struct dirent	*entry;
	struct stat		statbuf;
	char			*pathf;
	char			*f;
	char			*directory;
}	t_ffdr;

typedef struct s_cmd
{
	int				cmdn;
	int				isbltin;
	int				fd[2];
	int				pid;
	int				pidstatus;
	char			*cmd;
	char			*path;
	char			*lim;
	char			*infile;
	t_fout			**outfiles;
	int				fdinf;
	int				ifouts;
	int				priorinflim;
	int				priorinfout;
	char			**argv;
	char			**env;
	int				argc;
	struct s_cmd	*next;
	struct s_cmd	*prev;
}	t_cmd;

typedef struct mini
{
	int						argc;
	char					**argv;
	char					**envp;
	char					*infile;
	int						didcheckenv;
	int						status;
	t_cmd					**header;
	t_env					*env;
	t_env					**quotestmps;
}	t_mini;

/* ft_splitchats.c */
char	**ft_splitchars(char *str, char *charset);
/* minish.c */
void	closebutstds(int fd);
int		recursiva(t_mini **mini);
int		recread(t_mini **mini);
int		alonecmdcall(int fdin, t_cmd *cmd, char **env, t_mini *mini);
//		main();
/* minish2.c */
void	newfileout(t_fout **head, char *file, int app, int prior);
void	alonecmdcallutils(t_cmd *cmd, int fdin);
int		spacesindex(const char *str);
int		cmdexistence(char *cmd, t_mini *mini);
void	fdtofd(int fdin, int fdout);
/* minish3.c */
/* minish4.c */
void	closeanddupinput(int fd[2]);
void	closeanddupoutput(int fd[2]);
void	limmitator(char *lim, int fdin, struct mini *mini);
void	freelist(t_env *lst);
/* fileshandle.c */
int		anyfdtofile(int fd, t_fout *out, t_cmd *cmd, t_mini *mini);
void	fdtomfiles(t_fout **head, int fd, t_mini *mini, t_cmd *cmd);
void	freeoutfiles(t_fout **lst);
int		filesearch(t_fout *tmp, t_mini *mini);
t_fout	*foutlast(t_fout *lst);
/* fileshandle2.c */
char	*fileseek(t_ffdr *var);
char	*dir_seek(char *target_dir, char *directory);
char	*pathbuilder(char *dir, char *file);
void	putcmdn(t_cmd **head);
int		seekcasebar(t_fout *tmp, char *tmp2, char *tmp3, t_mini *mini);
/* fileshandle3.c */
char	*fileseekutils(t_ffdr *var);
char	*dirseekutils(t_ffdr *var);
void	freeffdr(t_ffdr *var);
/* minish6.c */
char	*pathseekenv(char **args, char **envp);
int		counttmps(t_env *lst);
void	putoutfn(t_fout **head);
/* minish8.c */
char	*checktmpslist(t_mini *mini, char **buf, char *tmp);
int		checkprepaths(char **cmd, t_mini *mini);
/* signals.c */
void	set_signals(void);
void	handle_sigint(int sig);
void	new_prompt(void);
/* builtins.c */
int		docd(t_cmd *cmd, t_mini *mini);
void	doecho(t_cmd *cmd, int fd);
int		doexport(t_mini *mini, t_cmd *cmd, int fd);
void	dounset(t_mini *mini, t_cmd	*cmd);
int		builtins(t_mini *mini, t_cmd *cmd);
/* builtins2.c */
int		is_valid_identifier(char *buf);
int		are_numbers(char *buf);
int		checkkill(char *buf);
void	chdirandoldpwd(char *new, t_mini *mini);
int		checkpermission(char *file, int rwx, t_mini *mini, t_cmd *actcmd);
/* builtins3.c */
char	*prevpath(char *path);
char	*prevcwd(void);
int		checkpermouts(t_cmd *cmd, char *file, t_mini *mini);
int		isbuiltin(t_cmd *cmd);
int		tildecasecd(t_env *tmp, t_mini *mini, char *path);
/* builtins4.c */
int		doexit(t_cmd *cmd);
int		donewvarent(t_cmd *cmd, t_mini *mini);
/* environment1.c */
t_env	*envarlast(t_env *lst);
char	*checkenvlist(t_mini *mini, char **buf, char *tmp);
int		entvars(t_env **head, char *var, char *content);
char	*checkenvvars(char *buf, t_mini *mini);
int		exists_env_var(t_mini *mini, char *varname);
/* environment2.c */
t_env	*get_env_head(t_env *env);
t_env	*get_env_var(t_env **head, char *varname);
t_env	*init_env_vars(char **envp);
void	add_temp_envar(t_mini *mini, char *varname);
void	add_envar(t_mini *mini, char *varname, char *value, int is_temp);
/* environment3.c */
void	remove_envar(t_mini *mini, char *varname);
void	print_temp_env(t_env *env, int fd);
void	print_envfd(t_env *env, int fd);
int		envsize(t_env *env);
char	**envtodoublechar(t_env *env);
/* executor.c */
int		run_cmd_list(t_mini *mini, t_cmd **head);
int		execute_command(t_mini *mini, t_cmd *cmd, int infd);
int		cmdcount(t_cmd **head);
int		dolimitator(char *lim, t_mini *mini);
void	postexec(t_cmd **head, t_mini *mini);
/* executor2.c */
void	fileunlinker(char *file);
void	closecmdsfd(t_cmd **head);
int		selectinflim(t_cmd *cmd, t_mini *mini);
void	openoutferrinf(t_cmd *cmd, t_mini *mini);
/* evaluator.c */
t_cmd	*evaluate_commands(char **args);
void	assign_outfile(t_cmd **current, char **args, int *begin, int app);
void	assign_infile(t_cmd **current, char **args, int *begin);
void	assignarg(t_cmd **cmd, char **args, int *begin);
t_cmd	*get_current_cmd(char **args, int *begin);
/* evaluator2.c */
int		is_operator(char *buf);
int		check_operator_syntax(char **args);
void	print_cmd_list(t_cmd *head);
void	free_cmd(t_cmd *cmd);
void	free_cmd_list(t_cmd **head);
/* evaluator3.c */
char	*argsearch(char *file);
void	argsfilesearcher(t_cmd **head);
char	*caseargsearch(t_ffdr *var, char *tp2, char *file, char *tmp3);
void	casenoopevals(char **args, int *i, t_cmd **current, int *tmp);
t_cmd	*caseisopevals(t_cmd **head, char **args, int *i, int *tmp);
/* parsing.c */
char	**process_input(t_mini *mini, char *buf);
int		count_splitted_operators(char *buf);
void	is_in_quotes(int *mode, char c);
char	**split_operators(int count, char *buf);
void	process_operators(char ***cmd);
/* parsing2.c */
void	split_input(char *buf, char ***cmd);
int		count_params(char *buf);
char	*process_vars(t_mini *mini, char *buf);
char	*remove_quotes(char *buf);
char	*expand_var(t_mini *mini, char *str, char *name);
/* parsing3.c */
char	*replace_content(char *str, char *content, char *start, int len);
int		get_namelen(char *name);
char	**cleannulls(char **prev);
/* cmdlisthandle.c */
void	cmdadd_back(t_cmd **lst, t_cmd *new);
t_cmd	*cmdlast(t_cmd *lst);
t_fout	*outfilelast(t_fout *lst);
t_fout	*outfilesearchbyfile(char *file, t_fout *lst);
int		outfcount(t_fout **head);

#endif
