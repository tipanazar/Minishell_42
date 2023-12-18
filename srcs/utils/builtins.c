#include "../../minishell.h"

void	change_to_home_directory(char **custom_environ)
{
	char	*home_dir;

	home_dir = custom_getenv("HOME", custom_environ, 0);
	if (home_dir)
	{
		if (chdir(home_dir) == 0)
			g_exit_code = 0;
		else
			g_exit_code = 1;
	}
	else
	{
		ft_printf("minishell# cd: HOME not set\n");
		g_exit_code = 1;
	}
}

void	change_to_specified_directory(char *new_buf)
{
	struct stat	file_stat;

	if (stat(new_buf, &file_stat) == 0)
	{
		if (S_ISDIR(file_stat.st_mode))
		{
			if (chdir(new_buf) == 0)
				g_exit_code = 0;
			else
				g_exit_code = 1;
		}
		else
		{
			ft_printf("minishell# cd: %s: Not a directory\n", new_buf);
			g_exit_code = 1;
		}
	}
	else
	{
		ft_printf("minishell# cd: %s: No such file or directory\n", new_buf);
		g_exit_code = 1;
	}
}

void	ft_cd(char *buf, char **custom_environ)
{
	char	*new_buf;

	new_buf = ft_str_remove_chars(buf, " ");
	if (ft_strlen(new_buf) == 0)
		change_to_home_directory(custom_environ);
	else
		change_to_specified_directory(new_buf);
	free(new_buf);
}

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

void	unset(char *buf, char ***custom_environ)
{
	char	**to_delete;
	int		idx;

	to_delete = create_unset_arr(buf, *custom_environ);
	if (!to_delete)
		return ;
	idx = 0;
	while (to_delete[idx])
	{
		ft_remove_str_from_char_arr(custom_environ, to_delete[idx]);
		idx++;
	}
	ft_free_char_arr(to_delete);
	g_exit_code = 0;
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
		g_exit_code = 0;
		return (1);
	}
	if (ft_strncmp(buf, "echo ", 5) == 0 || ft_strcmp(buf, "echo") == 0)
	{
		echo(buf + 4, custom_environ);
		return (1);
	}
	return (0);
}

// struct s_cmd	*parsepipe(char **ps, char *es)
// {
// 	struct s_cmd	*cmd;

// 	cmd = parseexec(ps, es);
// 	if (peek(ps, es, "|"))
// 	{
// 		get_token(ps, es, 0, 0);
// 		cmd = pipecmd(cmd, parsepipe(ps, es));
// 	}
// 	return (cmd);
// }
