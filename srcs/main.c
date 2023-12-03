#include "../minishell.h"

void ctrl_c_handler(int sig)
{
	(void)sig;
	rl_on_new_line();
	ft_printf("minishell#\n");
	rl_replace_line("", 0);
	rl_on_new_line();
	rl_redisplay();
}

bool is_blank(const char *buf)
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

void add_argument(struct s_execcmd *cmd, char *arg)
{
	if (cmd->argv == NULL)
	{
		cmd->argv = malloc(INITIAL_MAXARGS * sizeof(char *));
		cmd->max_args = INITIAL_MAXARGS;
		cmd->argc = 0;
	}
	else if (cmd->argc >= cmd->max_args)
	{
		cmd->max_args *= 2;
		cmd->argv = ft_realloc(cmd->argv, cmd->max_args * sizeof(char *));
	}
	cmd->argv[cmd->argc] = ft_strdup(arg);
	cmd->argc++;
}

void free_arguments(struct s_execcmd *cmd)
{
	for (int i = 0; i < cmd->argc; i++)
	{
		free(cmd->argv[i]);
	}
	free(cmd->argv);
}

int main(int ac, char **av, char **env)
{
	(void)ac;
	(void)av;
	char *buf;
	int r;
	char **custom_environ = malloc(sizeof(char *) * (ft_strarrlen(env) + 1));
	// custom_environ[0] = NULL;
	int idx = -1;
	while (env[++idx])
		custom_environ[idx] = ft_strdup(env[idx]);
	custom_environ[idx] = NULL;
	idx = -1;
	// while (custom_environ[++idx])
	// 	ft_printf("%s\n", custom_environ[idx]);

	signal(SIGINT, ctrl_c_handler);
	signal(SIGQUIT, SIG_IGN);
	while (1)
	{
		buf = readline("minishell# ");
		if (!buf)
			break;
		if (strcmp(buf, "exit") == 0)
		{
			free(buf);
			break;
		}
		if (strlen(buf) && isspace(buf[0]) == 0)
			add_history(buf);
		if (!buf || is_blank(buf))
		{
			free(buf);
			continue;
		}
		if (fork1() == 0)
		{
			runcmd(parsecmd(buf), custom_environ);
			free(buf);
			buf = NULL;
			exit(0);
		}
		wait(&r);
		free(buf);
	}
	rl_clear_history();
	ft_free_char_arr(custom_environ);
	return (0);
}