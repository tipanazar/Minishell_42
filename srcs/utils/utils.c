#include "../../minishell.h"

char	*check_for_pipes(char *buffer)
{
	int	i;

	i = 0;
	while (buffer && buffer[i] != '\0')
	{
		if (buffer[i] == '|')
		{
			if (buffer[i + 1] == '\0' || (buffer[i] == '|' && i == 0))
			{
				write(2, "syntax error near unexpected token `|'\n", 40);
				return (NULL);
			}
		}
		i++;
	}
	return (buffer);
}

char	*read_and_trim_line(char *buf)
{
	char	*new_buf;

	if (!buf)
		return (NULL);
	new_buf = ft_strtrim(buf, "\t\n\v\f ");
	free(buf);
	return (new_buf);
}

void	ctrl_c_handler(int sig)
{
	pid_t	pid;
	int		status;

	pid = waitpid(-1, &status, WNOHANG);
	if (sig == SIGINT)
	{
		if (pid == -1)
		{
			g_exit_code = 130;
			write(STDOUT_FILENO, "\n", 1);
			rl_on_new_line();
			rl_replace_line("", 0);
			rl_redisplay();
		}
		else
			write(STDOUT_FILENO, "\n", 1);
	}
}

bool	is_blank(const char *buf)
{
	if (!buf)
		return (true);
	while (*buf)
	{
		if (!ft_isspace(*buf))
			return (false);
		buf++;
	}
	return (true);
}
