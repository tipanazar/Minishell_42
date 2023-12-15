#include "../../minishell.h"

void echo(char *buf, char **custom_environ)
{
	int newline;
	int idx;
	int s_idx;
	int inside_sing_quotes;
	char *substr;
	char *getenv_result;

	idx = -1;
	s_idx = 1;
	inside_sing_quotes = 0;
	ft_trim_leading_spaces(buf);
	newline = ft_strncmp(buf, "-n", 2);
	if (!newline)
	{
		idx++;
		while (buf[++idx] == 'n')
			;
		if (!ft_isspace(buf[idx]))
		{
			newline = 1;
			idx = -1;
		}
		ft_trim_leading_spaces(buf);
	}
	while (buf[++idx])
	{
		if (buf[idx] == '\"')
			continue ;
		if (buf[idx] == '\'')
		{
			inside_sing_quotes = !inside_sing_quotes;
			continue ;
		}
		if (buf[idx] == '$' && !inside_sing_quotes && buf[idx + 1] && buf[idx
			+ 1] != '$' && !ft_isspace(buf[idx + 1]))
		{
			// ft_printf("%d", g_exit_code);
			if (buf[idx + 1] == '?')
			{
				ft_printf("%s", "*g_exit_code*");
				idx++;
			}
			else
			{
				while (buf[idx + s_idx] && !ft_isspace(buf[idx + s_idx + 1]))
					s_idx++;
				substr = ft_substr(buf, idx, s_idx);
				getenv_result = custom_getenv(substr + 1, custom_environ, 0);
				if (getenv_result)
					ft_printf("%s", getenv_result);
				free(substr);
				idx += s_idx;
			}
		}
		else
			ft_printf("%c", buf[idx]);
	}
	if (newline)
		ft_printf("\n");
}

char	*export_validator(char *buf)
{
	int		idx;
	bool	has_equal_sign;
	char	inside_quotes;
	char	*quote_type;

	has_equal_sign = false;
	inside_quotes = 0;
	quote_type = malloc(2);
	if (!quote_type)
	{
		perror("Memory allocation failed");
		exit(1);
	}
	quote_type[0] = 0;
	idx = -1;
	while (buf[++idx])
	{
		if (!ft_isspace(buf[idx]) && ft_isspace(buf[idx + 1]) && !inside_quotes
			&& has_equal_sign)
		{
			has_equal_sign = true;
			ft_printf("Trim\n");
			while (buf[++idx])
				buf[idx] = '\0';
			return (quote_type);
		}
		if (buf[idx] == '\'' || buf[idx] == '\"')
		{
			if (!inside_quotes)
				inside_quotes = buf[idx];
			else if (inside_quotes == buf[idx])
			{
				quote_type[0] = buf[idx];
				quote_type[1] = '\0';
				inside_quotes = 0;
			}
			else if (inside_quotes && inside_quotes != buf[idx] && !buf[idx
					+ 1])
			{
				ft_printf("Heredoc??\n");
				free(quote_type);
				return (NULL);
			}
		}
		if ((ft_isspace(buf[idx]) || ft_isdigit(buf[idx])) && buf[idx
			+ 1] == '=' && !has_equal_sign)
		{
			ft_printf("-minishell: export: `%s': not a valid identifier\n", buf
					+ idx + 1);
			free(quote_type);
			return (NULL);
		}
		if (buf[idx] == '=')
			has_equal_sign = true;
	}
	if (!has_equal_sign && (buf[idx - 1] == '\'' || buf[idx - 1] == '\"'))
	{
		ft_printf("Heredoc??\n");
		free(quote_type);
		return (NULL);
	}
	if (!has_equal_sign)
	{
		free(quote_type);
		return (NULL);
	}
	if (inside_quotes)
	{
		ft_printf("Heredoc??\n");
		free(quote_type);
		return (NULL);
	}
	return (quote_type);
}

void	export(char *buf, char ***custom_environ)
{
	char	*quote_type;
	char	*new_buf;
	int		idx;
	char	**splitted_environ;
	char	**splitted_new_buf;

	idx = -1;
	ft_trim_leading_spaces(buf);
	quote_type = export_validator(buf);
	if (!quote_type)
		return ;
	if (quote_type[0])
		new_buf = ft_str_remove_chars(buf, quote_type);
	else
		new_buf = ft_strdup(buf);
	free(quote_type);
	while ((*custom_environ)[++idx])
	{
		splitted_environ = ft_split((*custom_environ)[idx], '=');
		splitted_new_buf = ft_split(new_buf, '=');
		if (ft_strcmp(splitted_environ[0], splitted_new_buf[0]) == 0)
		{
			ft_strncpy((*custom_environ)[idx], new_buf, ft_strlen(new_buf));
			free(new_buf);
			ft_free_char_arr(splitted_environ);
			ft_free_char_arr(splitted_new_buf);
			return ;
		}
		ft_free_char_arr(splitted_environ);
		ft_free_char_arr(splitted_new_buf);
	}
	(*custom_environ) = realloc((*custom_environ), (idx + 2) * sizeof(char *));
	(*custom_environ)[idx] = ft_strdup(new_buf);
	(*custom_environ)[idx + 1] = NULL;
	free(new_buf);
}

char **create_unset_arr(char *buf, char **custom_environ)
{
	char *token;
	char *env_value;
	char **to_delete = NULL;
	int idx = 0;

	token = ft_strtok(buf, "\t\n\v\f ");

	while (token)
	{
		env_value = custom_getenv(token, custom_environ, 1);
		if (env_value)
		{
			to_delete = (char **)realloc(to_delete, sizeof(char *) * (ft_strarrlen(to_delete) + 2));
			to_delete[idx++] = ft_strdup(env_value);
			to_delete[idx] = NULL;
		}
		token = ft_strtok(NULL, "\t\n\v\f ");
	}
	return (to_delete);
}