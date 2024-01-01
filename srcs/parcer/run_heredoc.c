#include "../../minishell.h"

int ft_count_quotes(char *str)
{
	int count = 0;
	int i = -1;
	while (str[++i])
		if (str[i] == '\'' || str[i] == '\"')
			count++;

	return count;
}

char * process_delimeter(char *delimiter, char *quote_type)
{
	char to_remove[2] = {'\0', '\0'};
	if ((delimiter)[0] == '\'' || (delimiter)[0] == '\"')
		*quote_type = (delimiter)[0];
	else
		return ft_strdup(delimiter);
	if ((delimiter)[ft_strlen(delimiter) - 1] != *quote_type || ft_count_quotes(delimiter) % 2 != 0)
	{
		ft_printf("gimme last quote bruh🤡\n");
		return ft_strdup(delimiter);
	}
	to_remove[0] = *quote_type;
	return ft_str_remove_chars(delimiter, to_remove);
}

void process_heredoc(char *buf, char *delimiter, char **custom_environ)
{
	(void)custom_environ;
	(void)delimiter;
	(void)buf;
	// ft_printf("Buf: %s\nDelimiter: %s\n", buf, delimiter);
}

int read_heredoc_input(char *delimiter, int write_fd, char **custom_environ)
{
	char buffer[1024];
	size_t delimiter_length;
	ssize_t read_len;
	char quote_type = '\0';
	char *new_delimiter;

	new_delimiter = process_delimeter(delimiter, &quote_type);
	ft_printf("New delimiter: %s\n", new_delimiter);
	delimiter_length = ft_strlen(new_delimiter);
	while (1)
	{
		write(STDERR_FILENO, ">", 1);
		read_len = read(STDIN_FILENO, buffer, sizeof(buffer) - 1);
		if (read_len <= 0)
			break;
		buffer[read_len] = '\0';
		process_heredoc(buffer, new_delimiter, custom_environ);
		if (ft_strncmp(buffer, new_delimiter, delimiter_length) == 0 && (buffer[delimiter_length] == '\n' || buffer[delimiter_length] == '\0'))
			break;
		write(write_fd, buffer, read_len);
	}
	free(new_delimiter);
	return (1);
}

int double_redirect_left(struct s_redircmd *rcmd, char **custom_environ)
{
	int pipefd[2];

	if (pipe(pipefd) == -1)
	{
		perror("pipe");
		return (0);
	}
	if (!read_heredoc_input(rcmd->file, pipefd[1], custom_environ))
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

int handle_double_redirect_left(struct s_redircmd *rcmd, char ***custom_environ)
{
	int original_stdin;
	int pipe_read_end;

	original_stdin = dup(STDIN_FILENO);
	pipe_read_end = double_redirect_left(rcmd, *custom_environ);
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
