#include "../../minishell.h"

void	ft_cd(char *buf, char **custom_environ)
{
	char		*home_dir;
	char		*new_buf;
	struct stat	fileStat;

	new_buf = NULL;
	new_buf = ft_str_remove_chars(buf, " ");
	if (ft_strlen(new_buf) == 0)
	{
		home_dir = custom_getenv("HOME", custom_environ, 0);
		if (home_dir)
			chdir(home_dir);
		else
			ft_printf("-minishell: cd: HOME not set\n");
		return ;
	}
	if (stat(new_buf, &fileStat) || (S_ISDIR(fileStat.st_mode)
			&& chdir(new_buf) < 0))
		ft_printf("-minishell: cd: %s: No such file or directory\n", new_buf);
	else if (!S_ISDIR(fileStat.st_mode))
		ft_printf("-minishell: cd: %s: Not a directory\n", new_buf);
	free(new_buf);
}

void	pwd(void)
{
	char	dir[2000];

	if (getcwd(dir, sizeof(dir)) != NULL)
		ft_printf("%s\n", dir);
	else
		perror("pwd");
}

void	unset(char *buf, char ***custom_environ)
{
	char	**to_delete;
	int		idx;

	idx = -1;
	to_delete = create_unset_arr(buf, *custom_environ);
	while (to_delete[++idx])
		ft_remove_str_from_char_arr(custom_environ, to_delete[idx]);
	ft_free_char_arr(to_delete);
}

int	builtins(char *buf, char **custom_environ)
{
	if (ft_strncmp(buf, "pwd ", 4) == 0 || ft_strcmp(buf, "pwd") == 0)
	{
		pwd();
		return (1);
	}
	if (ft_strncmp(buf, "env ", 4) == 0 || ft_strcmp(buf, "env") == 0)
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
