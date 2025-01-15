/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipeline.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ipetrov <ipetrov@student.42bangkok.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/13 10:54:48 by ipetrov           #+#    #+#             */
/*   Updated: 2025/01/15 13:43:29 by ipetrov          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int run_cmd(t_cntx *cntx, char **argv);

//and mb create wrapper open_pipe()
void open_pipe(t_pipe *p)
{
	int fd[2];

	pipe(fd); //mb here handle errors??? //check error
	p->read = fd[0];
	p->write = fd[1];
}

//change to close_pipe()
void close_pipe(t_pipe *p)
{
	close(p->read);
	close(p->write);
}

int run_producer(t_cntx *cntx, int write, char **cmd) //to setup first only to be producer
{
	pid_t pid;

	pid = fork();
	if (pid == 0)
	{
		dup2(write, STDOUT_FILENO);
		run_cmd(cntx, cmd);
	}
	return (pid);
}

//for everyone between setup them to be producer and consumer
int run_mediators(t_cntx *cntx, int *read, char * * **cmd)
{
	pid_t pid;
	t_pipe p;

	// (*cmd)++;
	while (**cmd + 1)
	{
		(*cmd)++;
		open_pipe(&p);
		pid = fork();
		if (pid == 0)
		{
			dup2(*read, STDIN_FILENO);
			dup2(p.write, STDOUT_FILENO);
			run_cmd(cntx, **cmd); //close fd for pipe if cmd failed??
		}
		close_pipe(&(t_pipe){*read, p.write});
		*read = p.read;
	}
	return (pid);
}

int run_consumer(t_cntx *cntx, int read, char **cmd) //to setup last to be consumer
{
	pid_t pid;

	pid = fork();
	if (pid == 0)
	{
		dup2(read, STDIN_FILENO);
		run_cmd(cntx, cmd);
	}
	return (pid);
}

//make waitpid handler separate
int get_pipeline_exitcode(pid_t pid)
{
	int		status;
	pid_t	child;
	int		exitcode;

	child = 0;
	exitcode = -1;
	while (child != -1)
	{
		child = wait(&status);
		if (child == pid)
		{
			if (WIFEXITED(status))
			{
				exitcode = WEXITSTATUS(status);
			}
		}
	}
	return (exitcode);
}

char ***get_test();
//at least 2 commands as input
int run_pipeline(t_cntx *cntx, char * **cmd)
{
	t_pipe p;
	pid_t pid;
	int exitcode;

	cmd = get_test();
	//we are in original Bash or in Subshell for () for Others??
	open_pipe(&p);
	pid = run_producer(cntx, p.write, *cmd);//to setup first only to be producer // make them return not pid but cmd
	pid = run_mediators(cntx, &p.read, &cmd);
	pid = run_consumer(cntx, p.read, *cmd);
	close_pipe(&p);
	exitcode = get_pipeline_exitcode(pid);
	printf("Exitcode: %d\n", exitcode);
	//set it to $?
	//cat | cat | la | cat
	return (SUCCESS);
}

char ***get_test()
{
	char ***cmd = (char ***)malloc(5 * sizeof(char **)); // 5 commands in total (4 + NULL)

	cmd[0] = (char **)malloc(3 * sizeof(char *)); // 3 pointers for "ls", ".", NULL
	cmd[0][0] = "ls";
	cmd[0][1] = ".";
	cmd[0][2] = NULL;

	cmd[1] = (char **)malloc(2 * sizeof(char *)); // 2 pointers for "cat", NULL
	cmd[1][0] = "cat";
	cmd[1][1] = NULL;

	cmd[2] = (char **)malloc(2 * sizeof(char *)); // 2 pointers for "cat", NULL
	cmd[2][0] = "cat";
	cmd[2][1] = NULL;

	cmd[3] = (char **)malloc(2 * sizeof(char *)); // 2 pointers for "cat", NULL
	cmd[3][0] = "cat";
	cmd[3][1] = NULL;

	cmd[4] = NULL; // End of array

	return cmd;
}