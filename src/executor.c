/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mpenas-z <mpenas-z@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/29 14:21:20 by mpenas-z          #+#    #+#             */
/*   Updated: 2024/12/29 14:30:54 by mpenas-z         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/mini.h"

// This function iterates the list executing each command.
// If a command fails, output error and keep going.
// For some reason, in bash echo after a failed pipe prints before error.
// -> Should we store cmd outputs and then evaluate priority?
int	run_cmd_list(t_list *head)
{

}

// This function executes the current command once all pipes, 
// files and redirections have been set up.
int	execute_command(t_cmd *cmd, int infd, int outfd)
{

}
