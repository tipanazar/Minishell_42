#include "../minishell.h"

void	ft_cd(char *buf)
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

	if (getcwd(cwd, sizeof(cwd)) != NULL)
		printf("%s\n", cwd);
	else
		perror("getcwd() error");
}


int	builtins(char *buf)
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
	if(ft_strncmp(buf, "env", 3) == 0)
	{
		env();
		return (1);
	}
	// if (ft_strncmp(buf, "echo ", 5) == 0)
	//     ft_printf("%s\n", buf + 5);
	return (0);
}
