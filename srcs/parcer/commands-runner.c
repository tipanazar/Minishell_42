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
	int	flags;

	if (rcmd->type == '>')
		flags = O_WRONLY | O_CREAT | O_TRUNC;
	else if (rcmd->type == '<')
		flags = O_RDONLY;
	else
		flags = O_WRONLY | O_CREAT | O_APPEND;
	fd_redirect = open(rcmd->file, flags, 0666);
	if (fd_redirect < 0)
	{
		perror("open");
		exit(0);
	}
	if (dup2(fd_redirect, rcmd->fd) < 0)
	{
		perror("dup2");
		exit(0);
	}
	runcmd(rcmd->cmd);
	close(fd_redirect);
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
