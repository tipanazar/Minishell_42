#include "../minishell.h"

void	ft_cd(const char *buf)
{
	char		*home_dir;
	struct stat	fileStat;

	ft_trim_leading_spaces(buf + 2);
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

void	pwd(void)
{
	char	cwd[PATH_MAX];

	if (getcwd(cwd, sizeof(cwd)))
		printf("%s\n", cwd);
	else
		perror("getcwd() error");
}

void	echo(const char *buf)
{
	int	newline;
	int	idx;
	int	s_idx;
	int	sing_quotes;
	int	double_quotes;
	int	inside_sing_quotes;

	newline = 1;
	idx = -1;
	s_idx = 1;
	sing_quotes = 0;
	double_quotes = 0;
	inside_sing_quotes = 0;
	ft_trim_leading_spaces(buf);
	if (ft_strncmp(buf, "-n", 2) == 0)
	{
		newline = 0;
		buf += 2;
		ft_trim_leading_spaces(buf);
	}
	while (buf[++idx])
	{
		if (buf[idx] == '\'')
			sing_quotes++;
		if (buf[idx] == '\"')
			double_quotes++;
	}
	idx = -1;
	if (sing_quotes % 2 != 0 || double_quotes % 2 != 0)
	{
		ft_printf("Quotes amount is not even!\n");
		return ;
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
		if (buf[idx] == '$' && !inside_sing_quotes && !ft_isspace(buf[idx + 1]))
		{
			// if (buf[idx + 1] == '?')
			// 	ft_printf("%d", g_exit_code);
			// else
			// {
			while (buf[idx + s_idx] && !ft_isspace(buf[idx + s_idx]))
				s_idx++;
			ft_printf("%s", getenv(ft_substr(buf, idx + 1, s_idx)));
			idx += s_idx;
			// }
		}
		else
			ft_printf("%c", buf[idx]);
	}
	if (newline)
		ft_printf("\n");
}

int	builtins(const char *buf, char **environ)
{
	ft_trim_leading_spaces(buf);
	if (ft_strncmp(buf, "cd", 2) == 0)
	{
		ft_cd(buf);
		return (1);
	}
	if (ft_strncmp(buf, "pwd", 3) == 0)
	{
		pwd();
		return (1);
	}
	if (ft_strncmp(buf, "env", 3) == 0)
	{
		env(environ);
		return (1);
	}
	if (ft_strncmp(buf, "echo ", 4) == 0)
	{
		echo(buf + 4);
		return (1);
	}
	return (0);
}
