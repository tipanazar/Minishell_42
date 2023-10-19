#include "../minishell.h"

int	ft_cd(char *buf)
{
	int	flag;

	flag = 0;
	if (buf[0] == 'c' && buf[1] == 'd' && buf[2] == ' ')
		if (chdir(buf + 3) < 0)
			write(2, "cannot cd %s\n", 13);
	return (flag);
}

int	main(void)
{
	char	*buf;
	int		r;

	while (1)
	{
		buf = readline("minishell> ");
		if (!buf || ft_strcmp(buf, "exit") == 0)
			break ;
		if (ft_cd(buf))
			continue ;
		if (fork1() == 0)
		{
			add_history(buf);
			runcmd(parsecmd(buf));
			free(buf);
			exit(0);
		}
		wait(&r);
		free(buf);
	}
	return (0);
}
