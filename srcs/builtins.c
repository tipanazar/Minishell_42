#include "../minishell.h"

void ft_cd(char *buf, char **custom_environ)
{
	char *home_dir;
	char *new_buf;
	struct stat fileStat;

	new_buf = ft_str_remove_chars(buf, " ");
	if (!ft_strlen(new_buf))
	{
		home_dir = custom_getenv("HOME", custom_environ);
		if (home_dir)
			chdir(home_dir);
		free(home_dir);
	}
	else if (stat(new_buf, &fileStat) == 0)
	{
		if (S_ISDIR(fileStat.st_mode) && chdir(new_buf) < 0)
			ft_printf("-minishell: cd: %s: No such file or directory\n", new_buf);
		else if (!S_ISDIR(fileStat.st_mode))
			ft_printf("-minishell: cd: %s: Not a directory\n", new_buf);
	}
	else
		ft_printf("-minishell: cd: %s: No such file or directory\n", new_buf); //* in case of cd in a non-existing directory
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

char *getenv_custom(const char *name, char **custom_environ)
{

	int idx = -1;
	// ft_print_str_arr(custom_environ);
	while (custom_environ[++idx])
	{
		if (ft_strcmp(ft_split(custom_environ[idx], '=')[0], name) == 0)
			return custom_environ[idx] + ft_strlen(name) + 1;
	}

	return NULL;
}

void echo(char *buf, char **custom_environ)
{
	int newline;
	int idx;
	int s_idx;
	// int	inside_sing_quotes;

	idx = -1;
	s_idx = 1;
	// // inside_sing_quotes = 0;
	ft_trim_leading_spaces(buf);
	newline = ft_strncmp(buf, "-n", 2);
	if (!newline)
	{
		buf += 2;
		ft_trim_leading_spaces(buf);
	}
	// ft_printf("%s", buf);
	while (buf[++idx])
	{
		// if (buf[idx] == '\"')
		// 	continue;
		// if (buf[idx] == '\'')
		// {
		// 	inside_sing_quotes = !inside_sing_quotes;
		// 	continue;
		// }
		// if (buf[idx] == '$' && !inside_sing_quotes && !ft_isspace(buf[idx + 1]))
		if (buf[idx] == '$' && !ft_isspace(buf[idx + 1]))
		{
			// if (buf[idx + 1] == '?')
			// 	ft_printf("%d", g_exit_code);
			// else
			// {
			while (buf[idx + s_idx] && !ft_isspace(buf[idx + s_idx]))
				s_idx++;
			ft_printf("%s", getenv_custom(ft_substr(buf, idx + 1, s_idx), custom_environ));
			idx += s_idx;
			// }
		}
		else
			ft_printf("%c", buf[idx]);
	}
	if (newline)
		ft_printf("\n");
}

char *concat_args(char **args)
{
	int idx;
	char *str;

	idx = -1;
	str = "";
	while (args[++idx])
	{
		str = ft_strjoin(str, args[idx]);
		if (args[idx + 1])
			str = ft_strjoin(str, " ");
	}
	return (str);
}

char *export_validator(char *buf)
{
	int idx;
	bool has_equal_sign = false;
	char inside_quotes = 0;
	char *quote_type = malloc(sizeof(char) * 2);
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
				exit(0);
			}
		}
		if ((ft_isspace(buf[idx]) || ft_isdigit(buf[idx])) && buf[idx + 1] == '=' && !has_equal_sign)
		{
			ft_printf("-minishell: export: `%s': not a valid identifier\n", buf + idx + 1);
			exit(0);
		}
		if (buf[idx] == '=')
			has_equal_sign = true;
	}
	if (!has_equal_sign)
		exit(0);
	if (inside_quotes)
	{
		ft_printf("Heredoc??\n");
		exit(0);
	}
	return quote_type;
}

void export(char *buf, char **custom_environ) //? HOME=lox -> HME=lox
{
	char *quote_type;
	char *new_buf;
	int idx;
	idx = -1;
	ft_trim_leading_spaces(buf);
	quote_type = export_validator(buf);
	new_buf = ft_str_remove_chars(buf, quote_type); //* possible leak
	free(quote_type);

	while (custom_environ[++idx])
	{
		if (ft_strcmp(ft_split(custom_environ[idx], '=')[0], ft_split(new_buf, '=')[0]) == 0)
		{
			ft_printf("Free: %s\n", custom_environ[idx]);
			free(custom_environ[idx]);
			custom_environ[idx] = ft_strdup(new_buf);
			free(new_buf);
			return;
		}
	}
	custom_environ[idx] = ft_strdup(new_buf);
	custom_environ[idx + 1] = NULL;
	ft_printf("Done: %s\n", custom_environ[idx]);
	ft_print_str_arr(custom_environ);
	free(new_buf);
}

int builtins(char *buf, char **custom_environ)
{

	if (ft_strncmp(buf, "pwd", 3) == 0 && ft_strlen(buf) == 3)
	{
		pwd();
		free(buf);
		return (1);
	}
	if (ft_strncmp(buf, "env", 3) == 0 && ft_strlen(buf) == 3) //!!
	{
		ft_print_str_arr(custom_environ);
		free(buf);
		return (1);
	}
	if (ft_strncmp(buf, "echo ", 5) == 0)
	{
		echo(buf + 4, custom_environ);
		free(buf);
		return (1);
	}
	return (0);
}
