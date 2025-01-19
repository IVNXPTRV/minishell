/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ipetrov <ipetrov@student.42bangkok.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/13 13:36:16 by ipetrov           #+#    #+#             */
/*   Updated: 2025/01/15 17:18:43 by ipetrov          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

// # include "elibft.h"
# include <stdlib.h>
# include <unistd.h>
# include <stdio.h>

typedef struct s_cntx
{
	char **envp;
} t_cntx;

typedef struct s_pipe
{
	int read;
	int write;
} t_pipe;


typedef enum e_quotes_type
{
	BARE,
	SINGLE,
	DOUBLE
} t_quotes_type;

typedef enum e_token_type
{
	AND,
	OR,
	LPAR,
	PIPE,
	ARG,
	REDIR_IN,
	REDIR_OUT,
	REDIR_APPEND,
	REDIR_HEREDOC,
	RPAR
}	t_token_type;

//const
typedef enum e_error
{
	MALLOC,
	CMD_NOT_FOUND,
	ERRNO,
	FORK,
	OPEN,
	DUP2,
	GNL,
	FILE_NOT_FOUND,

}	t_error;

typedef struct s_token
{
	char 			*content;
	t_token_type	type;
	t_quotes_type	quotes; //0 - no quotes, 1 - ', 2 - "
	struct s_token *next;
} t_token;

typedef struct s_node
{
	struct s_token	*token;
	struct s_node 	*left;
	struct s_node 	*right;
} t_node;

//execution
int run_cmd(t_cntx *cntx, char **argv);
char *get_validpath(t_cntx *cntx, char **argv);
char *get_varvalue(t_cntx *cntx, char *varname);

//redirection
int	redir_in(char *pathname);

//error
void error(t_cntx *cntx, t_error error);

#endif