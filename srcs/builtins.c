#include "../minishell.h"

void env(char **environ)
{
	char **env;

	env = environ;
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

char *getenv_custom(const char *name, char **environ)
{
	for (char **env = environ; *env != NULL; env++)
	{
		char *env_name = strtok(*env, "=");
		char *env_value = strtok(NULL, "=");

		if (env_name && env_value && strcmp(env_name, name) == 0)
			return env_value;
	}

	return NULL;
}

void echo(char *buf, char **environ)
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
			ft_printf("%s", getenv_custom(ft_substr(buf, idx + 1, s_idx), environ));
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

int do_variable(char *name, char *value, char **environ)
{
	int idx;
	idx = -1;

	while (name[++idx])
		if (ft_isspace(name[idx]))
			return 0;
	idx = -1;
	while (environ[++idx])
	{
		if (ft_strcmp(ft_split(environ[idx], '=')[0], name) == 0)
		{
			free(environ[idx]);
			environ[idx] = ft_strjoin(ft_strjoin(name, "="), value);
			return 1;
		}
	}

	environ[idx] = ft_strjoin(ft_strjoin(name, "="), value);
	ft_printf("Done: %s\n", environ[idx]);
	return 1;
}

int builtins(char *buf, char **environ)
{
	ft_trim_leading_spaces(buf);
	char **splitted = ft_split(buf, '=');
	if (ft_strchr(buf, '=') && do_variable(splitted[0], buf + ft_strlen(splitted[0] + 1), environ))
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
		env(environ);
		return (1);
	}
	if (ft_strncmp(buf, "echo", 4) == 0)
	{
		echo(buf + 4, environ);
		return (1);
	}
	return (0);
}
