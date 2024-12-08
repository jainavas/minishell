/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jainavas <jainavas@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/14 17:22:26 by jainavas          #+#    #+#             */
/*   Updated: 2024/12/08 15:19:42 by mpenas-z         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_H
# define PIPEX_H
# define READ_FD 0
# define WRITE_FD 1

# include <unistd.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <stdio.h>
# include <fcntl.h>
# include <stdlib.h>
# include <string.h>
# include <errno.h>
# include "../libft_ext/libft.h"
# include "../libft_ext/ft_printf.h"
# include "mini.h"

typedef struct pipex
{
	int			pid;
	int			**fd;
	int			fdout;
	int			fdin;
	char		**paths;
	char		*buf;
	char		*output;
	char		***cmds;
	int			numcmds;
	int			actcmd;
	int			out;
	int			app;
	char		**envp;
	char		*lim;
	char		*pwd;
	char		*filein;
	struct mini	*mini;
}	t_pipex;

/* pipex.c */
void		closeanddupinput(int fd[2]);
int			lastcmdcall(t_pipex *var, char **cmd, char *path);
int			firstcmdcall(t_pipex *var, char **cmd, char *path);
int			vardefs(t_pipex *vars, char **argv, int argc);
/* pipex2.c */
void		freepipex(t_pipex *vars);
char		*pathseek(char **args, char **envp);
int			checks(char **argv, t_pipex *var);
void		fdtofile(t_pipex *var, char *filename);
void		closeanddupoutput(int fd[2]);
/* pipex3.c */
void		triplepointeralloc(t_pipex *vars, int argc);
int			checkpaths(t_pipex *vars);
int			normalcmdcall(t_pipex *var, char **cmd, char *path);
void		limmitator(char *lim, int fdin);
/* pipex4.c */
int			limvardefs(t_pipex *vars, char **argv, int argc);
char		*pwdseek(t_pipex *var);
int			limornot(int argc, char **argv, t_pipex *vars);
// void		anyfdtofile(int fd, char *filename, int app);

#endif
