#include "../minishell.h"

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