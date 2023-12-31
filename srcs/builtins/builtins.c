#include "../../minishell.h"

void	pwd(void)
{
	char	dir[2000];

	if (getcwd(dir, sizeof(dir)) != NULL)
	{
		ft_printf("%s\n", dir);
		g_exit_code = 0;
	}
	else
	{
		perror("pwd");
		g_exit_code = 1;
	}
}

void unset(char **buf_arr, char ***custom_environ)
{
    int idx;
    idx = -1;
    char *to_delete;
    while (buf_arr[++idx])
    {
        to_delete = custom_getenv(buf_arr[idx], *custom_environ, true);
        if(to_delete)
            ft_remove_str_from_char_arr(custom_environ, to_delete);
        to_delete = NULL;
    }
    g_exit_code = 0;
}

int builtins_part2(char **buf_args, int argc, char ***custom_environ)
{
	if (ft_strcmp(buf_args[0], "unset") == 0)
	{
		unset(buf_args + 1, custom_environ);
		return (1);
	}
	if (ft_strcmp(buf_args[0], "export") == 0)
	{
		if (argc == 1)
			ft_printf("-minishell: export: no arguments provided\n");
		else
			export(buf_args + 1, custom_environ);
		return (1);
	}
	if (ft_strcmp(buf_args[0], "cd") == 0)
	{
		if(argc > 2)
			ft_printf("-minishell: cd: too many arguments\n");
		else
			ft_cd(buf_args[1], *custom_environ);
		return (1);
	}
	return (0);
} 

int	builtins(char **buf_args, int argc, char ***custom_environ)
{
	if (ft_strcmp(buf_args[0], "pwd") == 0)
	{
		pwd();
		return (1);
	}
	if (ft_strcmp(buf_args[0], "env") == 0)
	{
		ft_print_str_arr(*custom_environ);
		g_exit_code = 0;
		return (1);
	}
	if (ft_strcmp(buf_args[0], "echo") == 0)
	{
		echo(buf_args + 1);
		return (1);
	}
	if(builtins_part2(buf_args, argc, custom_environ))
		return (1);
	return (0);
}
