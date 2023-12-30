#include "../../minishell.h"

int	read_heredoc_input(const char *delimiter, int write_fd)
{
	char	buffer[1024];
	size_t	delimiter_length;
	ssize_t	read_len;

	delimiter_length = ft_strlen(delimiter);
	while (1)
	{
		write(STDERR_FILENO, ">", 1);
		read_len = read(STDIN_FILENO, buffer, sizeof(buffer) - 1);
		if (read_len <= 0)
			break ;
		buffer[read_len] = '\0';
		if (ft_strncmp(buffer, delimiter, delimiter_length) == 0
			&& (buffer[delimiter_length] == '\n'
				|| buffer[delimiter_length] == '\0'))
			break ;
		write(write_fd, buffer, read_len);
	}
	return (1);
}

int	double_redirect_left(struct s_redircmd *rcmd)
{
	int	pipefd[2];

	if (pipe(pipefd) == -1)
	{
		perror("pipe");
		return (0);
	}
	if (!read_heredoc_input(rcmd->file, pipefd[1]))
	{
		close(pipefd[0]);
		close(pipefd[1]);
		return (0);
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
