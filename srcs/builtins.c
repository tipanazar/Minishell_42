#include "../minishell.h"

void env(char **custom_environ)
{
	char **env;

	env = custom_environ;
	while (*env)
	{
		printf("%s\n", *env);
		env++;
	}
}

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
	ft_print_char_arr(custom_environ);
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

int do_variable(char *name, char *value, char **custom_environ)
{
	int idx;
	idx = -1;

	while (name[++idx])
		if (ft_isspace(name[idx]))
			return 0;
	idx = -1;
	while (custom_environ[++idx])
	{
		if (ft_strcmp(ft_split(custom_environ[idx], '=')[0], name) == 0)
		{
			ft_printf("Free: %s\n", custom_environ[idx]);
			free(custom_environ[idx]);
			custom_environ[idx] = ft_strjoin(ft_strjoin(name, "="), value);
			idx++;
			return 1;
		}
	}
	custom_environ[idx] = ft_strjoin(ft_strjoin(name, "="), value);
	custom_environ[idx + 1] = NULL;
	// ft_printf("Done:\n\n");
	// ft_print_char_arr(custom_environ);
	return 1;
}

int builtins(char *buf, char **custom_environ)
{
	ft_trim_leading_spaces(buf);
	char **splitted = ft_split(buf, '=');
	if (ft_strchr(buf, '=') && do_variable(splitted[0], buf + ft_strlen(splitted[0]) + 1, custom_environ))
	{
		ft_free_char_arr(splitted);
		return 1;
	}
	else
		ft_free_char_arr(splitted);
	if (ft_strncmp(buf, "cd", 2) == 0)
	{
		ft_cd(buf + 2);
		return (1);
	}
	if (ft_strncmp(buf, "pwd", 3) == 0)
	{
		pwd();
		ft_printf("Return \n");
		return (1);
	}
	if (ft_strncmp(buf, "env", 3) == 0)
	{
		env(custom_environ);
		return (1);
	}
	if (ft_strncmp(buf, "echo", 4) == 0)
	{
		echo(buf + 4, custom_environ);
		return (1);
	}
	return (0);
}
