#include "../../minishell.h"

int	exec_cmd(struct execcmd *ecmd)
{
	if (ecmd->argv[0] == 0)
		exit(0);
	execvp(ecmd->argv[0], ecmd->argv);
	return (1);
}

int	redirect_cmd(struct redircmd *rcmd)
{
	int	fd_redirect;
	int	mode;

	fd_redirect = open(rcmd->file, rcmd->mode);
	if (rcmd->type == '>')
		mode = 0666;
	else
		mode = 0;
	if (fd_redirect < 0 || fd_redirect != mode)
	{
		write(STDERR_FILENO, "open ", 5);
		write(STDERR_FILENO, rcmd->file, ft_strlen(rcmd->file));
		write(STDERR_FILENO, " has failed\n", 11);
		exit(0);
	}
	if (dup2(fd_redirect, rcmd->fd) < 0)
	{
		write(2, "dup2 has failed\n", 15);
		exit(0);
	}
	runcmd(rcmd->cmd);
	return (1);
}

int	pipe_cmd(struct pipecmd *pcmd)
{
	int	fd_pipe[2];
	int	p_id;

	if (pipe(fd_pipe) < 0)
		exit(0);
	p_id = fork();
	if (p_id < 0)
		exit(0);
	else if (p_id == 0)
	{
		close(fd_pipe[1]);
		dup2(fd_pipe[0], STDIN_FILENO);
		runcmd(pcmd->right);
		close(fd_pipe[0]);
	}
	else
	{
		close(fd_pipe[0]);
		dup2(fd_pipe[1], STDOUT_FILENO);
		runcmd(pcmd->left);
		close(fd_pipe[1]);
		wait(&p_id);
	}
	return (1);
}

int	runcmd(struct cmd *cmd)
{
	char	type;

	if (cmd == 0)
		exit(0);
	type = cmd->type;
	if (type == ' ')
		return (exec_cmd((struct execcmd *)cmd));
	else if (type == '>' || type == '<')
		return (redirect_cmd((struct redircmd *)cmd));
	else if (type == '|')
		return (pipe_cmd((struct pipecmd *)cmd));
    else    
	    ft_printf("unknown runcmd\n");
	return (1);
}
