/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minish.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jainavas <jainavas@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/09 01:58:02 by jainavas          #+#    #+#             */
/*   Updated: 2025/01/28 19:38:23 by mpenas-z         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	g_signal;

void	closebutstds(int fd)
{
	if (fd > 2 && fd != -1)
		close(fd);
}

int	recursiva(t_mini **mini)
{
	int		x;

	(*mini)->infile = NULL;
	(*mini)->didcheckenv = 0;
	x = recread(mini);
	while (x == 0)
	{
		set_status((*mini)->status, mini);
		freelist(*(*mini)->quotestmps);
		*(*mini)->quotestmps = NULL;
		(*mini)->infile = NULL;
		(*mini)->didcheckenv = 0;
		x = recread(mini);
	}
	return (x);
}

int	recread(t_mini **mini)
{
	t_cmd	*head;
	char	**buf;
	char	*buf2;

	buf2 = readline("minishell% ");
	if (g_signal == 130)
		set_status(130, mini);
	if (!buf2)
		return (1);
	if (buf2[0] == '\0')
		return (free(buf2), 0);
	add_history(buf2);
	buf = cleannulls(process_input((*mini), ft_strdup(buf2)));
	head = evaluate_commands(buf, *mini);
	freedoublepointer(buf);
	if (!head)
		return (set_status((*mini)->status, mini), free(buf2), 0);
	(*mini)->header = &head;
	run_cmd_list(*mini, &head);
	free(buf2);
	free_cmd_list(&head);
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
		if (fdin > 2 && fdin != -1)
			close(fdin);
		return (close(cmd->fd[WRITE_FD]), cmd->fd[READ_FD]);
	}
	return (0);
}

int	main(int argc, char **argv, char **envp)
{
	t_mini	*mini;
	int		ret;

	g_signal = 0;
	mini = ft_calloc(1, sizeof(t_mini));
	mini->argc = argc;
	mini->argv = argv;
	mini->status = 0;
	mini->envp = envp;
	mini->env = init_env_vars(envp);
	add_envar(mini, "?", "0", 2);
	mini->quotestmps = ft_calloc(1, sizeof(t_env *));
	*(mini->quotestmps) = NULL;
	set_signals();
	recursiva(&mini);
	ret = mini->status;
	freelist(mini->env);
	freelist(*mini->quotestmps);
	if (ret != -1)
		return (free(mini->quotestmps), free(mini), ret);
	if (g_signal != -1)
		return (free(mini->quotestmps), free(mini), g_signal);
	return (free(mini->quotestmps), free(mini), 0);
}
