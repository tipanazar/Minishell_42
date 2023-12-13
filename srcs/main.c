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

int main(int ac, char **av, char **env)
{
	(void)ac;
	(void)av;
	struct s_cmd *cmd;
	char *buf;
	char *new_buf;
	int r;
	int idx = -1;
	char **custom_environ = (char **)malloc(sizeof(char *) * (ft_strarrlen(env) + 1));
	if (!custom_environ)
	{
		perror("Memory allocation failed");
		return (1);
	}
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
		if (ft_strncmp(new_buf, "exit ", 5) == 0 || ft_strcmp(new_buf, "exit") == 0)
		{
			free(new_buf);
			break;
		}
		if (is_blank(new_buf))
		{
			free(new_buf);
			continue;
		}
		if (ft_strlen(new_buf) && ft_isspace(new_buf[0]) == 0)
			add_history(new_buf);
		if (ft_strcmp(new_buf, "export") == 0 || ft_strncmp(new_buf, "export ", 7) == 0)
		{
			export(new_buf + 7, &custom_environ);
			free(new_buf);
			continue;
		}
		if (ft_strcmp(new_buf, "unset") == 0 || ft_strncmp(new_buf, "unset ", 6) == 0)
		{
			unset(new_buf + 5, &custom_environ);
			free(new_buf);
			continue;
		}
		if (ft_strcmp(new_buf, "cd") == 0 || ft_strncmp(new_buf, "cd ", 3) == 0)
		{
			ft_cd(new_buf + 2, custom_environ);
			free(new_buf);
			continue;
		}
		if (fork1() == 0)
		{
			cmd = parsecmd(new_buf);
			free(new_buf);
			runcmd(cmd, custom_environ);
			continue;
		}
		wait(&r);
		free(new_buf);
	}
	rl_clear_history();
	ft_free_char_arr(custom_environ);
	return (0);
}