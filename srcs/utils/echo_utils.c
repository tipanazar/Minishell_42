#include "../../minishell.h"

void	echo_n_handler(char *buf, int *idx, int *newline)
{
	ft_trim_leading_spaces(buf);
	if (ft_strncmp(buf, "-n", 2) == 0)
	{
		*newline = 0;
		*idx += 2;
	}
	else
		*newline = 1;
	if (*newline == 0)
	{
		(*idx)++;
		while (buf[*idx] == 'n')
			(*idx)++;
		if (!ft_isspace(buf[*idx]) && buf[*idx] != '\0')
		{
			*newline = 1;
			*idx = -1;
		}
		ft_trim_leading_spaces(buf);
	}
}

int	process_quotes(char *buf, int *idx, int *inside_sing_quotes)
{
	if (buf[*idx] == '\"' || buf[*idx] == '\'')
	{
		if (buf[*idx] == '\'')
			*inside_sing_quotes = !*inside_sing_quotes;
		(*idx)++;
		return (1);
	}
	return (0);
}

void	handle_variable_expansion(char *buf, char **custom_environ, int *idx)
{
	int		s_idx;
	char	*substr;
	char	*getenv_result;

	if (buf[*idx + 1] == '?')
	{
		ft_printf("%s", "*g_exit_code*");
		*idx += 2;
	}
	else
	{
		s_idx = 1;
		while (buf[*idx + s_idx] && !ft_isspace(buf[*idx + s_idx]) && buf[*idx
				+ s_idx] != '\0')
			s_idx++;
		substr = ft_substr(buf, *idx + 1, s_idx - 1);
		getenv_result = custom_getenv(substr, custom_environ, 0);
		if (getenv_result)
			ft_printf("%s", getenv_result);
		free(substr);
		*idx += s_idx;
	}
}

void	process_variables(char *buf, char **custom_environ, int *idx,
							int inside_sing_quotes)
{
	if (buf[*idx] == '$' && !inside_sing_quotes && buf[*idx + 1] != '\0'
		&& buf[*idx + 1] != '$' && !ft_isspace(buf[*idx + 1]))
		handle_variable_expansion(buf, custom_environ, idx);
	else
	{
		ft_printf("%c", buf[*idx]);
		(*idx)++;
	}
}

void	process_echo_command(struct s_echo_args *args)
{
	while (args->buf[*args->idx] != '\0')
	{
		if (!process_quotes(args->buf, args->idx, args->inside_sing_quotes))
			process_variables(args->buf, args->custom_environ, args->idx,
				*args->inside_sing_quotes);
	}
	if (args->newline)
		ft_printf("\n");
}
