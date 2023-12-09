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

// int main(void) {
//     char *test = strdup("test \"strin'g'");
//     printf("before: %s\n", test);
//     test = ft_str_remove_chars(&test, "\"'");
//     printf("after: %s\n", test);
//     free(test);

//     return 0;
// } //* to test ft_str_remove_chars

// int main(void)
// {
// 	char *new;
// 	char *test = strdup("    	test \"strin'g'   ");
// 	printf("before: %s\n", test);
// 	new = ft_strtrim(test, "\f\t ");
// 	free(test);
// 	printf("after: %s\n", new);
// 	free(new);

// 	return 0;
// } //* to test ft_strtrim

int main(int ac, char **av, char **env)
{
	(void)ac;
	(void)av;
	char *buf;
	char *new_buf;
	int r;
	int idx = -1;
	char **custom_environ = (char **)malloc(sizeof(char *) * (ft_strarrlen(env) + 1));
	while (env[++idx])
		custom_environ[idx] = ft_strdup(env[idx]);
	custom_environ[idx] = NULL;

	signal(SIGINT, ctrl_c_handler);
	signal(SIGQUIT, SIG_IGN);
	while (1)
	{
		buf = readline("minishell# ");
		if (!buf)
			break;
		new_buf = ft_strtrim(buf, "\f\t ");
		free(buf);
		if (ft_strncmp(new_buf, "exit ", 5) == 0)
		{
			free(new_buf);
			break;
		}
		if (ft_strlen(new_buf) && ft_isspace(new_buf[0]) == 0)
			add_history(new_buf);
		if (!new_buf || is_blank(new_buf))
		{
			free(new_buf);
			continue;
		}
		if (ft_strcmp(new_buf, "export") == 0 || ft_strncmp(new_buf, "export ", 7) == 0)
		{
			export(new_buf + 7, custom_environ);
			free(new_buf);
			continue;
		}
		if (ft_strcmp(new_buf, "cd") == 0 || ft_strncmp(new_buf, "cd ", 3) == 0)
		{
			ft_cd(new_buf + 2, custom_environ);
			free(new_buf);
			continue;
		}
		else if (fork1() == 0)
		{
			runcmd(parsecmd(new_buf), custom_environ);
			free(new_buf);
			exit(0); //* for what??
		}
		wait(&r);
		free(new_buf);
	}
	rl_clear_history();
	ft_free_char_arr(custom_environ);
	return (0);
}