/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minish.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jainavas <jainavas@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/09 01:58:02 by jainavas          #+#    #+#             */
/*   Updated: 2025/01/20 16:29:16 by jainavas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	g_status;

int	anyfdtofile(int fd, t_fout *out, t_cmd *cmd, t_mini *mini)
{
	int		fdo;
	int		r;

	fdo = 1;
	r = checkpermouts(cmd, out->file, mini);
	if (r == 1)
	{
		if (out->appendout == 1)
			fdo = open(out->file, O_WRONLY | O_APPEND);
		else
			fdo = open(out->file, O_WRONLY);
	}
	else if (r == -1)
		return (-1);
	else if (out->file)
		fdo = open(out->file, O_RDWR | O_CREAT | O_TRUNC, 0644);
	if (outfcount(cmd->outfiles) == out->foutn && fdo != 1 && fd != -1)
		fdtofd(fd, fdo);
	else if (fdo != 1)
		close(fdo);
	return (0);
}

int	alonecmdcall(int fdin, t_cmd *cmd, char **env, t_mini *mini)
{
	cmd->path = pathseekenv(&cmd->cmd, env);
	cmd->env = env;
	pipe(cmd->fd);
	cmd->pid = fork();
	if (cmd->pid == 0)
	{
		alonecmdcallutils(cmd, fdin);
		if (cmdcount(mini->header) != 1 || *cmd->outfiles != NULL)
			dup2(cmd->fd[WRITE_FD], STDOUT_FILENO);
		closeanddupinput(cmd->fd);
		execve(cmd->path, cmd->argv, cmd->env);
	}
	else
	{
		if (g_status == 130)
			write(1, "\n", 1);
		if (fdin > 2 && fdin != -1)
			close(fdin);
		return (close(cmd->fd[WRITE_FD]), cmd->fd[READ_FD]);
	}
	return (0);
}

char	**preppipex(char *buf, char *infile, char **buf2, t_mini *mini)
{
	char	**res;
	int		i;

	i = -1;
	res = ft_calloc((ft_strcount(buf, '|') + 1) + 4, sizeof(char *));
	res[1] = infile;
	res[0] = ft_strdup("a");
	while (buf2[++i])
	{
		if (checkprepaths(ft_split(buf2[i], ' '), mini))
			return (ft_putstr_fd("mini: command not found\n", 2),
				freedoublepointer(buf2), freedoublepointer(res), NULL);
		res [i + 2] = ft_strdup(buf2[i]);
	}
	res[i + 2] = NULL;
	return (freedoublepointer(buf2), res);
}

int	main(int argc, char **argv, char **envp)
{
	t_mini	*mini;
	int		ret;

	g_status = 0;
	mini = ft_calloc(1, sizeof(t_mini));
	mini->argc = argc;
	mini->argv = argv;
	mini->status = 0;
	mini->envp = envp;
	mini->env = init_env_vars(envp);
	add_envar(mini, "?", "0", 2);
	mini->mfilesout = ft_calloc(1, sizeof(t_fout *));
	*(mini->mfilesout) = NULL;
	mini->quotestmps = ft_calloc(1, sizeof(t_env *));
	*(mini->quotestmps) = NULL;
	set_signals();
	recursiva(&mini);
	ret = mini->status;
	freelist(mini->env);
	freelist(*mini->quotestmps);
	freeoutfiles(mini->mfilesout);
	free(mini->mfilesout);
	free(mini->quotestmps);
	free(mini);
	if (ret != -1)
		return (ret);
	if (g_status != -1)
		return (g_status);
	return (0);
}
