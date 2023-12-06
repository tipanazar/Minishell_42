#include "../minishell.h"

void ft_cd(char *buf)
{
	char *home_dir;
	struct stat fileStat;

	ft_trim_leading_spaces(buf);
	if (ft_strlen(buf) == 2)
	{
		home_dir = getenv("HOME");
		if (home_dir)
			chdir(home_dir);
	}
	else if (stat(buf + 2, &fileStat) == 0)
	{
		if (S_ISDIR(fileStat.st_mode) && chdir(buf + 2) < 0)
			ft_printf("cd: %s: No such file or directory\n", buf + 2);
		else if (!S_ISDIR(fileStat.st_mode))
			ft_printf("cd: %s: Not a directory\n", buf + 2);
	}
	else
		ft_printf("cd: %s: No such file or directory\n", buf + 2);
}

void pwd(void)
{
	char cwd[PATH_MAX];

	if (getcwd(cwd, sizeof(cwd)))
		printf("%s\n", cwd);
	else
		perror("getcwd() error");
	ft_printf("Return pwd \n");
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

void export_validator(char *buf)
{
	int idx;
	bool has_equal_sign = false;
	char inside_quotes = 0;
	idx = -1;

	while (buf[++idx])
	{
		if (!ft_isspace(buf[idx]) && ft_isspace(buf[idx + 1]) && !inside_quotes && has_equal_sign)
		{
			has_equal_sign = true;
			ft_printf("Trim\n");
			while (buf[++idx])
				buf[idx] = '\0';
			return;
		}
		if (buf[idx] == '\'' || buf[idx] == '\"')
		{
			if (!inside_quotes)
				inside_quotes = buf[idx];
			else if (inside_quotes == buf[idx])
				inside_quotes = 0;
			else if (inside_quotes && inside_quotes != buf[idx] && !buf[idx + 1])
			{
				ft_printf("Heredoc??\n");
				exit(0);
			}
		}
		if (ft_isspace(buf[idx]) && buf[idx + 1] == '=' && !has_equal_sign)
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
}

int export(char *buf, char **custom_environ)
{
	// int idx;
	// bool has_equal_sign = false;
	// idx = -1;
	(void)custom_environ;
	ft_trim_leading_spaces(buf);
	export_validator(buf);
	ft_printf("Buf: %s\n", buf);
	ft_printf("Ok\n");
	// while (custom_environ[++idx])
	// {
	// 	if (ft_strcmp(ft_split(custom_environ[idx], '=')[0], name) == 0)
	// 	{
	// 		ft_printf("Free: %s\n", custom_environ[idx]);
	// 		free(custom_environ[idx]);
	// 		custom_environ[idx] = ft_strjoin(ft_strjoin(name, "="), value);
	// 		idx++;
	// 		return 1;
	// 	}
	// }
	// custom_environ[idx] = ft_strjoin(ft_strjoin(name, "="), value);
	// custom_environ[idx + 1] = NULL;
	// // ft_printf("Done:\n\n");
	// // ft_print_str_arr(custom_environ);
	return 1;
}

int builtins(char *buf, char **custom_environ)
{
	if ((ft_strncmp(buf, "cd", 2) == 0 && ft_strlen(buf) == 2) || ft_strncmp(buf, "cd ", 3) == 0)
	{
		ft_cd(buf + 2);
		return (1);
	}
	if (ft_strncmp(buf, "pwd", 3) == 0 && ft_strlen(buf) == 3)
	{
		pwd();
		ft_printf("Return \n");
		return (1);
	}
	if (ft_strncmp(buf, "env", 3) == 0 && ft_strlen(buf) == 3) //!!
	{
		ft_print_str_arr(custom_environ);
		return (1);
	}
	if (ft_strncmp(buf, "echo ", 5) == 0)
	{
		echo(buf + 4, custom_environ);
		return (1);
	}
	return (0);
}
