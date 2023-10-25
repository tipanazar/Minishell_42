#include "../minishell.h"

void	ft_cd(char *buf)
{
	char	*home_dir;

	ft_trim_leading_spaces(buf + 2);
	if (ft_strlen(buf) == 2)
	{
		home_dir = getenv("HOME");
		if (home_dir)
			chdir(home_dir);
	}
	else if (chdir(buf + 2) < 0)
		ft_printf("cd: %s: No such file or directory\n", buf + 2);
}

int	builtins(char *buf)
{
	ft_trim_leading_spaces(buf);
	if (ft_strncmp(buf, "cd", 2) == 0)
	{
		ft_cd(buf);
		return (1);
	}
	// if (ft_strncmp(buf, "echo ", 5) == 0)
	//     ft_printf("%s\n", buf + 5);
	return (0);
}
