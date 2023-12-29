#include "../../minishell.h"

bool	process_quotes1(const char *buffer, size_t index, size_t len,
		bool quote_open, char quote_type)
{
	if (index >= len)
		return (quote_open);
	if (buffer[index] == '\"' || buffer[index] == '\'')
	{
		if (!quote_open || buffer[index] == quote_type)
		{
			quote_open = !quote_open;
			if (quote_open)
				quote_type = buffer[index];
			else
				quote_type = 0;
		}
	}
	return (process_quotes1(buffer, index + 1, len, quote_open, quote_type));
}

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

char	*check_for_quotes(char *buffer)
{
	size_t	len;

	len = ft_strlen(buffer);
	if (process_quotes1(buffer, 0, len, false, 0))
	{
		printf("Missing closing quote\n");
		return (NULL);
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
	(void)sig;
		g_exit_code = 130;
		rl_on_new_line();
		rl_redisplay();
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
