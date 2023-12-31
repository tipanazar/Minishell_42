#include "../../minishell.h"

void echo(char **buf_args)
{
	int newline;
	int idx = -1;
	newline = 1;
	echo_n_handler(buf_args[0], &newline);
	while (buf_args[++idx])
	{
		if(buf_args[idx + 1])
			ft_printf("%s ", buf_args[idx]);
		else
			ft_printf("%s", buf_args[idx]);
	}
	if (newline)
		ft_printf("\n");
	g_exit_code = 0;
}

char **create_unset_arr(char *buf, char **custom_environ)
{
	char *token;
	char **to_delete;
	int idx;

	to_delete = NULL;
	idx = 0;
	token = ft_strtok(buf, "\t\n\v\f ");
	while (token)
	{
		if (custom_getenv(token, custom_environ, 0))
		{
			to_delete = (char **)realloc(to_delete, sizeof(char *) * (idx + 2));
			if (!to_delete)
			{
				g_exit_code = 1;
				return (NULL);
			}
			to_delete[idx++] = ft_strdup(token);
			to_delete[idx] = NULL;
		}
		token = ft_strtok(NULL, "\t\n\v\f ");
	}
	return (to_delete);
}
