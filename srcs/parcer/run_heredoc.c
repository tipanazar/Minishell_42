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

void process_delimeter(char *delimiter, char *quote_type)
{
	char to_remove[2] = {'\0', '\0'};
	char *new_delimeter = NULL;
	int idx = -1;
	if (!ft_count_quotes(delimiter) || ft_strlen(delimiter) == 0)
		return;
	while (delimiter[++idx])
	{
		if (delimiter[idx] == '\'' || delimiter[idx] == '\"')
		{
			if (!(*quote_type))
				*quote_type = delimiter[idx];
			else if (*quote_type == delimiter[idx])
			{
				to_remove[0] = *quote_type;
				*quote_type = -1;
			}
		}
	}
	if (*quote_type > 0)
	{
		ft_printf("gimme last quote bruh🤡\n");
		return;
	}
	new_delimeter = ft_str_remove_chars(delimiter, to_remove);
	ft_strlcpy(delimiter, new_delimeter, ft_strlen(new_delimeter) + 1);
	free(new_delimeter);
} //! too big function, split it

char *return_variable(char *buf, int *idx, int *new_buf_idx, char **custom_environ)
{
	int s_idx;
	char *substr;
	char *getenv_result;
	s_idx = 1;
	while (buf[s_idx + *idx] && !ft_isspace(buf[s_idx + *idx]))
		s_idx++;
	substr = ft_substr(buf, *idx + 1, s_idx - 1);
	getenv_result = custom_getenv(substr, custom_environ, false);
	free(substr);
	*idx += s_idx - 1;
	if (getenv_result)
	{
		*new_buf_idx += ft_strlen(getenv_result);
		return getenv_result;
	}
	else
		return NULL;
}

int calc_len_with_vars(char *str, char **custom_environ)
{
	int idx = -1;
	int str_len = 0;
	char *substr;

	while (str[++idx])
	{
		if (str[idx] == '$')
		{
			if (str[idx + 1] == '?')
			{
				idx++;
				substr = ft_itoa(g_exit_code);
				str_len += ft_strlen(substr) + 1;
				free(substr);
			}
			else if (str[idx + 1] && ft_isdigit(str[idx + 1]))
			{
				idx++;
				str_len -= 1;
			}
			else if (str[idx + 1] && !ft_isspace(str[idx + 1]) && !ft_isdigit(str[idx + 1]))
				return_variable(str, &idx, &str_len, custom_environ);
		}
		str_len++;
	}
	return str_len;
} //! too big function, split it

char *process_heredoc(char *buf, char **custom_environ)
{
	int idx = -1;
	int s_idx = 0;
	char *substr;
	char *new_buf;
	new_buf = (char *)malloc(calc_len_with_vars(buf, custom_environ) + 1);
	if (!new_buf)
		return NULL;
	new_buf[0] = '\0';
	buf[ft_strlen(buf) - 1] = '\0';
	while (buf[++idx])
	{
		if (buf[idx] == '$')
		{
			if (buf[idx + 1] == '?')
			{
				idx++;
				substr = ft_itoa(g_exit_code);
				s_idx += ft_strlen(substr);
				ft_strcat(new_buf, substr);
				free(substr);
			}
			else if (buf[idx + 1] && ft_isdigit(buf[idx + 1]))
				idx++;
			else if (buf[idx + 1] && !ft_isspace(buf[idx + 1]) && !ft_isdigit(buf[idx + 1]))
			{
				substr = return_variable(buf, &idx, &s_idx, custom_environ);
				if (substr)
					ft_strcat(new_buf, substr);
			}
		}
		else
		{
			new_buf[s_idx++] = buf[idx];
			new_buf[s_idx] = '\0';
		}
	}
	new_buf[s_idx] = 10;
	new_buf[s_idx + 1] = '\0';
	return new_buf;
} //! too big function, split it

int read_heredoc_input(char *delimiter, int write_fd, char **custom_environ)
{
	char buffer[1024];
	ssize_t read_len;
	char quote_type = '\0';
	char *new_buf;

	process_delimeter(delimiter, &quote_type);
	while (1)
	{
		ft_putstr_fd("> ", STDERR_FILENO);
		read_len = read(STDIN_FILENO, buffer, sizeof(buffer) - 1);
		buffer[read_len] = '\0';
		if (!quote_type && read_len > 0)
			read_len = calc_len_with_vars(buffer, custom_environ);
		if (read_len <= 0 || ft_strcmp(buffer, delimiter) == 10)
			break;
		if (!quote_type)
		{
			new_buf = process_heredoc(buffer, custom_environ);
			ft_putstr_fd(new_buf, write_fd);
			free(new_buf);
		}
		else
			ft_putstr_fd(buffer, write_fd);
	}
	return (1);
} //! too big function, split it

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
