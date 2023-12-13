#include "../minishell.h"

void ft_cd(char *buf, char **custom_environ)
{
	char *home_dir;
	char *new_buf = NULL;
	struct stat fileStat;

	new_buf = ft_str_remove_chars(buf, " ");
	if (ft_strlen(new_buf) == 0)
	{
		home_dir = custom_getenv("HOME", custom_environ);
		if (home_dir)
			chdir(home_dir);
		else
			ft_printf("-minishell: cd: HOME not set\n");
		return;
	}
	if (stat(new_buf, &fileStat) || (S_ISDIR(fileStat.st_mode) && chdir(new_buf) < 0))
		ft_printf("-minishell: cd: %s: No such file or directory\n", new_buf);
	else if (!S_ISDIR(fileStat.st_mode))
		ft_printf("-minishell: cd: %s: Not a directory\n", new_buf);
	free(new_buf);
}

void pwd(void)
{
	char cwd[PATH_MAX];

	if (getcwd(cwd, sizeof(cwd)))
		printf("%s\n", cwd);
	else
		perror("getcwd() error");
}

void echo(char *buf, char **custom_environ) //? echo $$
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
			continue;
		if (buf[idx] == '\'')
		{
			inside_sing_quotes = !inside_sing_quotes;
			continue;
		}
		if (buf[idx] == '$' && !inside_sing_quotes && buf[idx + 1] && buf[idx + 1] != '$' && !ft_isspace(buf[idx + 1]))
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
				getenv_result = custom_getenv(substr + 1, custom_environ);
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

char *export_validator(char *buf)
{
	int idx;
	bool has_equal_sign = false;
	char inside_quotes = 0;
	char *quote_type = malloc(2);
	if (!quote_type)
	{
		perror("Memory allocation failed");
		exit(1);
	}
	quote_type[0] = 0;
	idx = -1;

	while (buf[++idx])
	{
		if (!ft_isspace(buf[idx]) && ft_isspace(buf[idx + 1]) && !inside_quotes && has_equal_sign)
		{
			has_equal_sign = true;
			ft_printf("Trim\n");
			while (buf[++idx])
				buf[idx] = '\0';
			return quote_type;
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
			else if (inside_quotes && inside_quotes != buf[idx] && !buf[idx + 1])
			{
				ft_printf("Heredoc??\n");
				free(quote_type);
				return NULL;
			}
		}
		if ((ft_isspace(buf[idx]) || ft_isdigit(buf[idx])) && buf[idx + 1] == '=' && !has_equal_sign)
		{
			ft_printf("-minishell: export: `%s': not a valid identifier\n", buf + idx + 1);
			free(quote_type);
			return NULL;
		}
		if (buf[idx] == '=')
			has_equal_sign = true;
	}
	if (!has_equal_sign && (buf[idx - 1] == '\'' || buf[idx - 1] == '\"'))
	{
		ft_printf("Heredoc??\n");
		free(quote_type);
		return NULL;
	}
	if (!has_equal_sign)
	{
		free(quote_type);
		return NULL;
	}
	if (inside_quotes)
	{
		ft_printf("Heredoc??\n");
		free(quote_type);
		return NULL;
	}
	return quote_type;
}

void export(char *buf, char ***custom_environ)
{
	char *quote_type;
	char *new_buf;
	int idx;
	char **splitted_environ;
	char **splitted_new_buf;
	idx = -1;
	ft_trim_leading_spaces(buf);
	quote_type = export_validator(buf);
	if (!quote_type)
		return;
	if (quote_type[0])
	{
		ft_printf("Quote type: %s\n", quote_type);
		new_buf = ft_str_remove_chars(buf, quote_type);
	}
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
			return;
		}
		ft_free_char_arr(splitted_environ);
		ft_free_char_arr(splitted_new_buf);
	}

	(*custom_environ) = realloc((*custom_environ), (idx + 2) * sizeof(char *));
	(*custom_environ)[idx] = ft_strdup(new_buf);
	(*custom_environ)[idx + 1] = NULL;
	ft_printf("Done: %s\n", (*custom_environ)[idx]);
	free(new_buf);
}

void unset(char *buf, char ***custom_environ)
{
	int f_idx = -1;
	int s_idx = 0;
	char *new_buf = ft_str_remove_chars(buf, "\"\'");
	ft_trim_leading_spaces(new_buf);
	// char **splitted_buf = ft_strtok(new_buf, "\t\n\v\f ");
	// ft_print_str_arr(splitted_buf);
	// while()
	(void)custom_environ;
	(void)f_idx;
	(void)s_idx;
}

int builtins(char *buf, char **custom_environ)
{

	if (ft_strncmp(buf, "pwd ", 4) == 0 || ft_strcmp(buf, "pwd") == 0)
	{
		pwd();
		return (1);
	}
	if (ft_strncmp(buf, "env ", 4) == 0 || ft_strcmp(buf, "pwd") == 0)
	{
		ft_print_str_arr(custom_environ);
		return (1);
	}
	if (ft_strncmp(buf, "echo ", 5) == 0 || ft_strcmp(buf, "echo") == 0)
	{
		echo(buf + 4, custom_environ);
		return (1);
	}
	return (0);
}
