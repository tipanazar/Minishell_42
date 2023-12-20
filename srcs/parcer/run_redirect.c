#include "../../minishell.h"

int	double_redirect_left(struct s_redircmd *rcmd)
{
	char	buffer[1024];
	int		pipefd[2];
	size_t	delimiter_length;
	ssize_t	read_len;

	write(STDERR_FILENO, rcmd->file, ft_strlen(rcmd->file));
	write(STDERR_FILENO, "\n", 1);	
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
	char *str;

	fd_redirect = open(rcmd->file, flags, 0666);
	if (fd_redirect < 0)
	{	
		str = ft_strjoin("minishell: ", rcmd->file);
		perror(str);
		free(str);
		g_exit_code = 1;
		return (-1);
	}
	if (dup2(fd_redirect, rcmd->fd) < 0)
	{
		perror("dup2");
		close(fd_redirect);
		g_exit_code = 1;
		return (-1);
	}
	runcmd(rcmd->cmd, custom_environ);
	close(fd_redirect);
	return (1);
}

int	handle_double_redirect_left(struct s_redircmd *rcmd, char **custom_environ)
{
	int	pipe_read_end;

	pipe_read_end = double_redirect_left(rcmd);
	if (pipe_read_end < 0)
	{
		perror("double_redirect_left");
		g_exit_code = 1;
		return (-1);
	}
	runcmd(rcmd->cmd, custom_environ);
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

int	redirect_cmd(struct s_redircmd *rcmd, char **custom_environ)
{
	int	flags;

	flags = get_redirection_flags(rcmd->type);
	if (flags == -1 && rcmd->type != '-')
		return (-1);
	if (rcmd->type == '-')
		return (handle_double_redirect_left(rcmd, custom_environ));
	else
		return (handle_redirection(rcmd, custom_environ, flags));
}
