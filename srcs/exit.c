#include "../minishell.h"

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

int builtin_exit_middleware(char **args, int argc)
{
	if (argc > 2 || (argc == 2 && !is_valid_number(args[1])))
	{
		if (argc > 2)
			ft_putendl_fd("exit: too many arguments", 2);
		else
			ft_putendl_fd("exit: numeric argument required", 2);
		ft_free_char_arr(args);
		g_exit_code = 1;
		if (argc <= 2)
			g_exit_code = 2;
		return 1;
	}
	return 0;
}

int builtin_exit(char *buf)
{
	char **args = ft_split(buf, ' ');
	int argc;

	argc = ft_strarrlen(args);
	if (builtin_exit_middleware(args, argc))
		return 1;
	if (argc == 2)
		g_exit_code = ft_atoi(args[1]);
	if (g_exit_code < 0)
		g_exit_code = (256 + g_exit_code) % 256;
	else
		g_exit_code %= 256;
	ft_free_char_arr(args);
	return 0;
}
