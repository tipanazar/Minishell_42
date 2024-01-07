#include "../../minishell.h"

int	fuck_norm(int fd_redirect)
{
	if (fd_redirect < 0)
	{
		perror("open");
		g_exit_code = 1;
		return (-1);
	}
	return (0);
}

int	setup_redirection(struct s_redircmd *rcmd, int flags, int *saved_fd)
{
	int	fd_redirect;

	fd_redirect = open(rcmd->file, flags, 0666);
	if (fuck_norm(fd_redirect) != 0)
		return (-1);
	*saved_fd = dup(rcmd->fd);
	if (*saved_fd < 0)
	{
		perror("dup");
		close(fd_redirect);
		g_exit_code = 1;
		return (-1);
	}
	if (dup2(fd_redirect, rcmd->fd) < 0)
	{
		perror("dup2");
		close(fd_redirect);
		close(*saved_fd);
		g_exit_code = 1;
		return (-1);
	}
	close(fd_redirect);
	return (0);
}

int	execute_and_restore(struct s_redircmd *rcmd, char ***custom_environ,
		int saved_fd)
{
	if (rcmd->cmd)
		runcmd(rcmd->cmd, custom_environ);
	if (dup2(saved_fd, rcmd->fd) < 0)
	{
		perror("dup2");
		g_exit_code = 1;
		return (-1);
	}
	close(saved_fd);
	return (1);
}

int	handle_redirection(struct s_redircmd *rcmd, char ***custom_environ,
		int flags)
{
	int	saved_fd;
	int	result;

	result = setup_redirection(rcmd, flags, &saved_fd);
	if (result != 0)
		return (result);
	return (execute_and_restore(rcmd, custom_environ, saved_fd));
}

void	redirect_cmd(struct s_redircmd *rcmd, char ***custom_environ)
{
	int	flags;

	if (rcmd->type == '>')
		flags = O_WRONLY | O_CREAT | O_TRUNC;
	else if (rcmd->type == '<')
		flags = O_RDONLY;
	else if (rcmd->type == '+')
		flags = O_WRONLY | O_CREAT | O_APPEND;
	else if (rcmd->type == '-')
	{
		handle_double_redirect_left(rcmd, custom_environ);
		return ;
	}
	else
		return ;
	handle_redirection(rcmd, custom_environ, flags);
}
