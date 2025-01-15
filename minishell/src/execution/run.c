/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   run.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ipetrov <ipetrov@student.42bangkok.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/12 19:04:47 by ipetrov           #+#    #+#             */
/*   Updated: 2025/01/15 13:43:31 by ipetrov          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	redir_heredoc(char *delim);
int	redir_append(char *pathname);
int apply_redirs();

//close fd for pipe if cmd failed??
//this function runs already inside of fork if new process nedded
//it doesnt create new process
int run_cmd(t_cntx *cntx, char **argv)
{
	char *pathname;

	apply_redirs();
	//BUILT-IN: check here if cmd is built-in and run without execve


	pathname = get_validpath(cntx, argv);
	if (execve(pathname, argv, cntx->envp) == FAIL)
		error(cntx, CMD_NOT_FOUND); //here prnit not found and fully exit() to not allow to go throuw otehr code
	return (SUCCESS);
}
