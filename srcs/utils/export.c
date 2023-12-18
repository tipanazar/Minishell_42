#include "../../minishell.h"

char	*final_validation_checks(char *buf, int idx, t_ValidationArgs *args)
{
	if (!*(args->has_equal_sign) && (buf[idx - 1] == '\'' || buf[idx
				- 1] == '\"'))
	{
		ft_printf("Heredoc??\n");
		return (NULL);
	}
	if (!*(args->has_equal_sign) || *(args->inside_quotes))
	{
		ft_printf("Heredoc??\n");
		return (NULL);
	}
	return (args->quote_type);
}

bool	handle_space_and_equal(char *buf, int idx, bool *has_equal_sign,
		char *inside_quotes)
{
	if (!ft_isspace(buf[idx]) && ft_isspace(buf[idx + 1]) && !*inside_quotes
		&& *has_equal_sign)
	{
		*has_equal_sign = true;
		ft_printf("Trim\n");
		while (buf[++idx])
			buf[idx] = '\0';
		return (false);
	}
	if ((ft_isspace(buf[idx]) || ft_isdigit(buf[idx])) && buf[idx + 1] == '='
		&& !*has_equal_sign)
	{
		ft_printf("-minishell: export: `%s': not a valid identifier\n", buf
			+ idx + 1);
		return (false);
	}
	if (buf[idx] == '=')
		*has_equal_sign = true;
	return (true);
}

bool	check_quote_status(char *buf, int idx, char *inside_quotes,
		char *quote_type)
{
	if (buf[idx] == '\'' || buf[idx] == '\"')
	{
		if (!*inside_quotes)
		{
			*inside_quotes = buf[idx];
		}
		else if (*inside_quotes == buf[idx])
		{
			quote_type[0] = buf[idx];
			quote_type[1] = '\0';
			*inside_quotes = 0;
		}
		else if (*inside_quotes && *inside_quotes != buf[idx] && !buf[idx + 1])
		{
			ft_printf("Heredoc??\n");
			return (false);
		}
	}
	return (true);
}

char	*validate_buffer(char *buf, t_ValidationArgs *args)
{
	int	idx;

	idx = -1;
	while (buf[++idx])
	{
		if (!check_quote_status(buf, idx, args->inside_quotes,
				args->quote_type))
		{
			free(args->quote_type);
			return (NULL);
		}
		if (!handle_space_and_equal(buf, idx, args->has_equal_sign,
				args->inside_quotes))
		{
			free(args->quote_type);
			return (NULL);
		}
	}
	return (final_validation_checks(buf, idx, args));
}

void	export(char *buf, char ***custom_environ)
{
	char	*quote_type;
	char	*new_buf;
	int		idx;

	quote_type = export_validator(buf);
	if (!quote_type)
		return ;
	if (quote_type[0])
		new_buf = ft_str_remove_chars(buf, quote_type);
	else
		new_buf = ft_strdup(buf);
	free(quote_type);
	idx = -1;
	while ((*custom_environ)[++idx])
	{
		if (update_env_var(custom_environ, new_buf, idx))
		{
			free(new_buf);
			return ;
		}
	}
	add_new_env_var(custom_environ, new_buf, idx);
	free(new_buf);
	g_exit_code = 0;
}
