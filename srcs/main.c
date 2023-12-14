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

// void add_argument(struct s_execcmd *cmd, char *arg)
// {
// 	if (cmd->argv == NULL)
// 	{
// 		cmd->argv = malloc(INITIAL_MAXARGS * sizeof(char *));
// 		cmd->max_args = INITIAL_MAXARGS;
// 		cmd->argc = 0;
// 	}
// 	else if (cmd->argc >= cmd->max_args)
// 	{
// 		cmd->max_args *= 2;
// 		cmd->argv = ft_realloc(cmd->argv, cmd->max_args * sizeof(char *));
// 	}
// 	cmd->argv[cmd->argc] = ft_strdup(arg);
// 	cmd->argc++;
// } //? not used...

// void free_arguments(struct s_execcmd *cmd)
// {
// 	for (int i = 0; i < cmd->argc; i++)
// 	{
// 		free(cmd->argv[i]);
// 	}
// 	free(cmd->argv);
// } //? not used...

// int main(void) {
//     char *test = strdup("test \"strin'g'");
//     printf("before: %s\n", test);
//     test = ft_str_remove_chars(&test, "\"'");
//     printf("after: %s\n", test);
//     free(test);

//     return 0;
// } //? to test ft_str_remove_chars

// void ft_move(char **str)
// {
// 	char *move;
// 	move = *str;
// 	move += ft_strlen(*str) ;
// 	*str = ft_strdup(move);
// }

// int main(void)
// {
// 	char *new;
// 	new = ft_strtrim("    	test \"strin'g'   ", "\f\t ");
// 	ft_printf("Before: %s;\n", new);
// 	ft_move(&new);
// 	printf("After: %s;\n", new);
// 	free(new);

// 	return 0;
// } //? to test ft_strtrim

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
		new_buf = ft_strtrim(buf, "\t\n\v\f ");
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
			runcmd(cmd, custom_environ); //? волграйнд дивно працює, можливо треба вбити процесс при виході з builtins?
			continue;
		}
		wait(&r);
		free(new_buf);
	}
	rl_clear_history();
	ft_free_char_arr(custom_environ);
	return (0);
}