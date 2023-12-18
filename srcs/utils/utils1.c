#include "../../minishell.h"

int	fork1(void)
{
	int	pid;

	pid = fork();
	if (pid == -1)
		perror("fork");
	return (pid);
}

int	peek(char **ps, char *es, char *toks)
{
	char	*s;
	char	*whitespace;

	whitespace = " \t\r\n\v";
	s = *ps;
	while (s < es && ft_strchr(whitespace, *s))
		s++;
	*ps = s;
	return (*s && ft_strchr(toks, *s));
}

char	*mkcopy(char *s, char *es)
{
	int		n;
	char	*c;

	n = es - s;
	c = malloc(n + 1);
	if (c == NULL)
	{
		write(2, "Memory allocation failed.\n", 25);
		exit(1);
	}
	ft_strncpy(c, s, n);
	c[n] = 0;
	return (c);
}

char	*concat_args(char **args)
{
	int		idx;
	char	*str;
	char	*temp;

	idx = -1;
	str = "";
	while (args[++idx])
	{
		temp = ft_strjoin(str, args[idx]);
		if (ft_strlen(str))
			free(str);
		str = temp;
		if (args[idx + 1])
		{
			temp = ft_strjoin(str, " ");
			free(str);
			str = temp;
		}
	}
	return (str);
}

char	*custom_getenv(char *name, char **custom_environ, bool full_str)
{
	int	idx;
	int	len;

	idx = -1;
	len = ft_strlen(name);
	while (custom_environ[++idx])
	{
		if (ft_strncmp(custom_environ[idx], name, len) == 0)
		{
			if (full_str)
				return (custom_environ[idx]);
			else
				return (ft_strchr(custom_environ[idx], '=') + 1);
		}
	}
	return (NULL);
}
