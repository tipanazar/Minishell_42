#include "../minishell.h"

void	execute_command(char *new_buf, char **custom_env)
{
	pid_t			pid;
	int				r;
	struct s_cmd	*cmd;

	pid = fork();
	if (pid < 0)
	{
		perror("fork");
		exit(EXIT_FAILURE);
	}
	if (pid == 0)
	{
		cmd = parsecmd(new_buf);
		free(new_buf);
		runcmd(cmd, custom_env);
		ft_free_char_arr(custom_env);
		free_cmd(cmd);
		exit(EXIT_SUCCESS);
	}
	else
		wait(&r);
}

char	**clone_env(char **env)
{
	char	**custom_env;
	int		idx;

	idx = -1;
	custom_env = (char **)malloc(sizeof(char *) * (ft_strarrlen(env) + 1));
	if (!custom_env)
		return (NULL);
	while (env[++idx])
		custom_env[idx] = ft_strdup(env[idx]);
	custom_env[idx] = NULL;
	return (custom_env);
}

bool	handle_command(char *new_buf, char ***custom_env)
{
	if (ft_strlen(new_buf) && ft_isspace(new_buf[0]) == 0)
		add_history(new_buf);
	if (ft_strcmp(new_buf, "export") == 0 || ft_strncmp(new_buf, "export ",
			7) == 0)
	{
		export(new_buf + 7, custom_env);
		return (true);
	}
	else if (ft_strcmp(new_buf, "unset") == 0 || ft_strncmp(new_buf, "unset ",
			6) == 0)
	{
		unset(new_buf + 5, custom_env);
		return (true);
	}
	else if (ft_strcmp(new_buf, "cd") == 0 || ft_strncmp(new_buf, "cd ",
			3) == 0)
	{
		ft_cd(new_buf + 2, *custom_env);
		return (true);
	}
	return (false);
}

void	process_input(char **custom_env)
{
	char	*new_buf;

	while (1)
	{
		new_buf = read_and_trim_line();
		if (!new_buf)
			break ;
		if (ft_strcmp(new_buf, "exit") == 0 || ft_strncmp(new_buf, "exit ",
				5) == 0)
		{
			free(new_buf);
			break ;
		}
		if (is_blank(new_buf))
		{
			free(new_buf);
			continue ;
		}
		if (!handle_command(new_buf, &custom_env))
			execute_command(new_buf, custom_env);
		free(new_buf);
	}
	rl_clear_history();
	ft_free_char_arr(custom_env);
}

int	main(int ac, char **av, char **env)
{
	char	**custom_env;

	(void)ac;
	(void)av;
	signal(SIGINT, ctrl_c_handler);
	signal(SIGQUIT, SIG_IGN);
	custom_env = clone_env(env);
	process_input(custom_env);
	return (0);
}
