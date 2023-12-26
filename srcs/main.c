#include "../minishell.h"

int g_exit_code;

void execute_command(char *new_buf, char **custom_env)
{
	pid_t pid;
	int r;
	struct s_cmd *cmd;

	pid = fork();
	if (pid < 0)
	{
		perror("fork");
		exit(EXIT_FAILURE);
	}
	if (pid == 0)
	{
		signal(SIGQUIT, SIG_DFL);
		signal(SIGINT, SIG_DFL);
		cmd = parsecmd(new_buf);
		free(new_buf);
		if(cmd->flag != 1)
			runcmd(cmd, custom_env);
		ft_free_char_arr(custom_env);
		free_cmd(cmd);
		exit(g_exit_code);
	}
	else
	{
		wait(&r);
		if (WIFSIGNALED(r) && WTERMSIG(r) == SIGQUIT)
		{
			write(STDOUT_FILENO, "Quit (core dumped)\n", 19);
			g_exit_code = 131;
		}
		else if (WIFEXITED(r))
			g_exit_code = WEXITSTATUS(r);
	}
}

char **clone_env(char **env)
{
	char **custom_env;
	int idx;

	idx = -1;
	custom_env = (char **)malloc(sizeof(char *) * (ft_strarrlen(env) + 1));
	if (!custom_env)
		return (NULL);
	while (env[++idx])
		custom_env[idx] = ft_strdup(env[idx]);
	custom_env[idx] = NULL;
	return (custom_env);
}

bool handle_command(char *new_buf, char ***custom_env)
{
	if (ft_strlen(new_buf) && ft_isspace(new_buf[0]) == 0)
		add_history(new_buf);
	if (ft_strcmp(new_buf, "export") == 0 || ft_strncmp(new_buf, "export ",
														7) == 0)
	{
		if (ft_strncmp(new_buf, "export ", 7))
			ft_printf("-minishell: export: no arguments provided\n");
		else
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

int is_valid_number(char *str)
{
	if (!str)
		return 0;

	if (*str == '-' || *str == '+')
		str++;

	while (*str)
	{
		if (!ft_isdigit(*str))
			return 0;
		str++;
	}
	return 1;
}

void ft_put_exit_code(int argc, char **args)
{
	if (argc == 1 && !is_valid_number(args[0]))
		g_exit_code = 2;
	else if (argc == 1)
		g_exit_code = ft_atoi(args[0]);
	else if (g_exit_code < 0)
		g_exit_code = (256 + g_exit_code) % 256;
	else
		g_exit_code %= 256;
}

int builtin_exit(char *buf)
{
	char **args;
	int argc = 0;

	ft_trim_leading_spaces(buf);
	args = ft_split(buf, ' ');
	while (args[argc])
		argc++;
	if (argc > 1 && is_valid_number(args[0]))
	{
		ft_printf("-minishell: exit: too many arguments\n");
		ft_free_char_arr(args);
		g_exit_code = 1;
		if (argc <= 2)
			g_exit_code = 2;
		return 1;
	}
	else if (argc > 1)
		ft_printf("-minishell: exit: numeric argument required\n");
	ft_put_exit_code(argc, args);
	ft_free_char_arr(args);
	return 0;
}

void process_input(char **custom_env)
{
	char *new_buf;
	char *buf;

	while (1)
	{
		buf = readline("minishell# ");
		new_buf = read_and_trim_line(buf);
		if (!new_buf)
			break ;
		if(check_for_quotes(new_buf) == NULL)
		{
			free(new_buf);
			continue ;
		}
		if (ft_strcmp(new_buf, "exit") == 0 || ft_strncmp(new_buf, "exit ",
														  5) == 0)
		{
			ft_printf("logout\n");
			if (!builtin_exit(new_buf + 4))
			{
				rl_clear_history();
				ft_free_char_arr(custom_env);
				ft_printf("Exit: %d\n", g_exit_code);
				free(new_buf);
				exit(g_exit_code);
			}
			free(new_buf);
			continue;
		}
		if (is_blank(new_buf))
		{
			free(new_buf);
			continue;
		}
		if (!handle_command(new_buf, &custom_env))
			execute_command(new_buf, custom_env);
		free(new_buf);
	}
	rl_clear_history();
	ft_free_char_arr(custom_env);
}

int main(int ac, char **av, char **env)
{
	char **custom_env;

	(void)ac;
	(void)av;
	g_exit_code = 0;
	signal(SIGINT, ctrl_c_handler);
	signal(SIGQUIT, SIG_IGN);
	custom_env = clone_env(env);
	process_input(custom_env);
	return (0);
}
