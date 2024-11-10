/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jainavas <jainavas@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/14 17:22:26 by jainavas          #+#    #+#             */
/*   Updated: 2024/11/10 00:50:48 by jainavas         ###   ########.fr       */
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
# include "libft_ext/libft.h"
# include "libft_ext/ft_printf.h"

typedef struct pipex
{
	int		pid;
	int		**fd;
	int		fdout;
	int		fdin;
	char	**paths;
	char	*buf;
	char	*output;
	char	***cmds;
	int		numcmds;
	int		actcmd;
	char	**envp;
}	t_pipex;

void		freepipex(t_pipex *vars);
char		*pathseek(char **args, char **envp);
void		closeanddupoutput(int fd[2]);
void		closeanddupinput(int fd[2]);
int			checks(char **argv, t_pipex *var);
void		fdtofile(t_pipex *var, char *filename);
int			lastcmdcall(t_pipex *var, char **cmd, char *path);
int			normalcmdcall(t_pipex *var, char **cmd, char *path);
int			firstcmdcall(t_pipex *var, char **cmd, char *path);
int			vardefs(t_pipex *vars, char **argv, int argc);
void		triplepointeralloc(t_pipex *vars, int argc);
int			checkpaths(t_pipex *vars);
int			pipex(int argc, char **argv, char **envp);
int			dstrlen(char **strofstr);

#endif