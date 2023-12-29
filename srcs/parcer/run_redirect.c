#include "../../minishell.h"

int	double_redirect_left(struct s_redircmd *rcmd)
{
	char	buffer[1024];
	int		pipefd[2];
	size_t	delimiter_length;
	ssize_t	read_len;

	if (rcmd == NULL || rcmd->file == NULL)
	{
		write(STDERR_FILENO, "Invalid command\n", 16);
		return (0);
	}
	if (pipe(pipefd) == -1)
	{
		perror("pipe");
		return (0);
	}
	delimiter_length = ft_strlen(rcmd->file);
	while (1)
	{
		write(STDERR_FILENO, ">", 1);
		read_len = read(STDIN_FILENO, buffer, sizeof(buffer) - 1);
		if (read_len <= 0)
			break ;
		buffer[read_len] = '\0';
		if (ft_strncmp(buffer, rcmd->file, delimiter_length) == 0 &&
			(buffer[delimiter_length] == '\n'
					|| buffer[delimiter_length] == '\0'))
			break ;
		write(pipefd[1], buffer, read_len);
	}
	close(pipefd[1]);
	if (dup2(pipefd[0], STDIN_FILENO) == -1)
	{
		perror("dup2");
		close(pipefd[0]);
		close(pipefd[1]);
		g_exit_code = 1;
		return (0);
	}
	close(pipefd[0]);
	return (1);
}

int	handle_redirection(struct s_redircmd *rcmd, char **custom_environ,
		int flags)
{
	int	fd_redirect;
	int	saved_fd;

	fd_redirect = open(rcmd->file, flags, 0666);
	if (fd_redirect < 0)
	{
		perror("open");
		g_exit_code = 1;
		return (-1);
	}
	saved_fd = dup(rcmd->fd);
	if (saved_fd < 0)
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
		close(saved_fd);
		g_exit_code = 1;
		return (-1);
	}
	if (rcmd->cmd)
		runcmd(rcmd->cmd, custom_environ);
	if (dup2(saved_fd, rcmd->fd) < 0)
		perror("dup2");
	close(saved_fd);
	close(fd_redirect);
	return (1);
}

int	handle_double_redirect_left(struct s_redircmd *rcmd, char **custom_environ)
{
	int	original_stdin;
	int	pipe_read_end;

	original_stdin = dup(STDIN_FILENO);
	pipe_read_end = double_redirect_left(rcmd);
	if (pipe_read_end < 0)
	{
		perror("double_redirect_left");
		dup2(original_stdin, STDIN_FILENO);
		close(original_stdin);
		g_exit_code = 1;
		return (-1);
	}
	runcmd(rcmd->cmd, custom_environ);
	dup2(original_stdin, STDIN_FILENO);
	close(original_stdin);
	return (1);
}

int	get_redirection_flags(char type)
{
	if (type == '>')
		return (O_WRONLY | O_CREAT | O_TRUNC);
	else if (type == '<')
		return (O_RDONLY);
	else if (type == '+')
		return (O_WRONLY | O_CREAT | O_APPEND);
	else
		return (-1);
}

void	redirect_cmd(struct s_redircmd *rcmd, char **custom_environ)
{
	int	flags;

	flags = get_redirection_flags(rcmd->type);
	if (flags == -1 && rcmd->type != '-')
		return ;
	if (rcmd->type == '-')
		handle_double_redirect_left(rcmd, custom_environ);
	else
		handle_redirection(rcmd, custom_environ, flags);
}
