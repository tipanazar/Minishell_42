#include "../minishell.h"

void	ctrl_c_handler(int sig)
{
	(void)sig;
	rl_on_new_line();
	ft_printf("minishell#\n");
	rl_replace_line("", 0);
	rl_on_new_line();
	rl_redisplay();
}

bool	is_blank(const char *buf)
{
	if (!buf)
		return (true);
	while (*buf)
	{
		if (!ft_isspace(*buf))
			return (false);
		buf++;
	}
	return (true);
}

int	handle_built_in_commands(char *new_buf, char ***custom_environ)
{
	if (ft_strncmp(new_buf, "exit ", 5) == 0 || ft_strcmp(new_buf, "exit") == 0)
		return (1);
	if (is_blank(new_buf))
		return (1);
	if (ft_strlen(new_buf) && ft_isspace(new_buf[0]) == 0)
	{
		add_history(new_buf);
	}
	if (ft_strcmp(new_buf, "export") == 0 || ft_strncmp(new_buf, "export ",
			7) == 0)
	{
		export(new_buf + 7, custom_environ);
		return (1);
	}
	if (ft_strcmp(new_buf, "unset") == 0 || ft_strncmp(new_buf, "unset ",
			6) == 0)
	{
		unset(new_buf + 5, custom_environ);
		return (1);
	}
	if (ft_strcmp(new_buf, "cd") == 0 || ft_strncmp(new_buf, "cd ", 3) == 0)
	{
		ft_cd(new_buf + 2, *custom_environ);
		return (1);
	}
	return (0); // Not a built-in command
}

// <<<<<<< turman
// char	**init_custom_environment(char **env)
// =======
// int main(int ac, char **av, char **env)
// {
// 	(void)ac;
// 	(void)av;
// 	int idx = -1;
// 	char **custom_environ = (char **)malloc(sizeof(char *) * (4));
// 	while (++idx < 3)
// 		custom_environ[idx] = ft_strdup(env[idx]);
// 	custom_environ[idx] = NULL;

// 	ft_printf("Before:\n");
// 	ft_print_str_arr(custom_environ);
// 	ft_remove_str_from_char_arr(&custom_environ, "SHELL=/bin/bash");
// 	ft_remove_str_from_char_arr(&custom_environ, "SHELL=/bin/bash");
// 	ft_remove_str_from_char_arr(&custom_environ, "WSL2_GUI_APPS_ENABLED=1");
// 	ft_remove_str_from_char_arr(&custom_environ, "WSL_DISTRO_NAME=Ubuntu-22.04");
// 	printf("After:\n");
// 	ft_print_str_arr(custom_environ);

// 	ft_free_char_arr(custom_environ);

// 	return 0;
// } //? to test ft_remove_str_from_char_arr

int main(int ac, char **av, char **env)
// >>>>>>> main
{
	int		idx;
	char	**custom_environ;

	idx = -1;
	custom_environ = (char **)malloc(sizeof(char *) * (ft_strarrlen(env) + 1));
	if (!custom_environ)
	{
		perror("Memory allocation failed");
		return (NULL);
	}
	while (env[++idx])
		custom_environ[idx] = ft_strdup(env[idx]);
	custom_environ[idx] = NULL;
	return (custom_environ);
}

void	process_commands(char **custom_environ)
{
	struct s_cmd	*cmd;
	int				r;

	char *buf, *new_buf;
	while (1)
	{
		buf = readline("minishell# ");
		if (!buf)
			break;
		new_buf = ft_strtrim(buf, "\t\n\v\f ");
		free(buf);
		if (handle_built_in_commands(new_buf, &custom_environ))
		{
			free(new_buf);
			continue ;
		}
		if (fork1() == 0)
		{
			cmd = parsecmd(new_buf);
			free(new_buf);
			runcmd(cmd, custom_environ);
				ft_free_char_arr(custom_environ);
			free_cmd(cmd);
			exit(1);
		}
		wait(&r);
		free(new_buf);
	}
}

void	setup_signal_handlers(void)
{
	signal(SIGINT, ctrl_c_handler);
	signal(SIGQUIT, SIG_IGN);
}

int	main(int ac, char **av, char **env)
{
	char	**custom_environ;

	(void)ac;
	(void)av;
	custom_environ = init_custom_environment(env);
	if (!custom_environ)
		return (1);
	setup_signal_handlers();
	process_commands(custom_environ);
	rl_clear_history();
	ft_free_char_arr(custom_environ);
	return (0);
}
