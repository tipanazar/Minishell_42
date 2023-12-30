#include "../minishell.h"

int		g_exit_code;

void	execute_command(char *new_buf, char **custom_env)
{
	struct s_cmd	*cmd;
	
	cmd = parsecmd(new_buf);
	free(new_buf);
	if (cmd->flag != 1)
		runcmd(cmd, custom_env);
	free_cmd(cmd);
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

void	process_input(char **custom_env)
{
	char	*new_buf;
	char	*buf;

	while (1)
	{
		buf = readline("My_Fuckingshell# ");
		new_buf = read_and_trim_line(buf);
		if (!new_buf)
			break ;
		if (check_for_pipes(new_buf) == NULL)
		{
			free(new_buf);
			continue ;
		}
		if (check_for_quotes(new_buf) == NULL)
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
			continue ;
		}
		if (is_blank(new_buf))
		{
			free(new_buf);
			continue ;
		}
		if (!handle_command(new_buf, &custom_env))
			execute_command(new_buf, custom_env);
	}
	rl_clear_history();
	ft_free_char_arr(custom_env);
}

int	main(int ac, char **av, char **env)
{
	char	**custom_env;

	(void)ac;
	(void)av;
	g_exit_code = 0;
	signal(SIGQUIT, SIG_IGN);
	signal(SIGINT, ctrl_c_handler);
	custom_env = clone_env(env);
	process_input(custom_env);
	return (0);
}
