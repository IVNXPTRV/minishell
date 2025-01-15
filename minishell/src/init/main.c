/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ipetrov <ipetrov@student.42bangkok.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/11 18:06:39 by ipetrov           #+#    #+#             */
/*   Updated: 2025/01/15 13:45:19 by ipetrov          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int run_pipeline(t_cntx *cntx, char * **cmd);

int init_cntx(t_cntx **cntx)
{
	*cntx = ft_calloc(1, sizeof(t_cntx));
	if (!*cntx)
		return (FAIL);
	return (SUCCESS);
}

int init_envp(t_cntx *cntx, char **envp)
{
	size_t	len;
	int		i;

	len = ft_parrlen(envp);
	cntx->envp = ft_calloc(len + 1, sizeof(char *));
	if (!cntx->envp)
		return (FAIL);
	i = 0;
	if (envp[i] == NULL)
	{
		cntx->envp[i] = NULL;
		return (SUCCESS);
	}
	while (i < len)
	{
		cntx->envp[i] = ft_strdup(envp[i]);
		if (!cntx->envp[i])
		{
			ft_parrclean(0, free, cntx->envp, NULL);
			return (FAIL);
		}
		i++;
	}
	cntx->envp[i] = NULL;
	return (SUCCESS);
}

int init(t_cntx **cntx, char **envp)
{
	if (init_cntx(cntx) == FAIL)
		return (FAIL);
	if (init_envp(*cntx, envp) == FAIL)
		return (FAIL);
	return (SUCCESS);
}

int	main(int argc, char **argv, char **envp)
{
	t_cntx *cntx;

	if (init(&cntx, envp) == FAIL)
	{
		return (EXIT_FAILURE);
	}
	run_pipeline(cntx, NULL);
	// run_cmd(cntx, cmd);
	return (EXIT_SUCCESS);
}
