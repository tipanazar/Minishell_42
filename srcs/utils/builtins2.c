#include "../../minishell.h"

void	echo(char *buf, char **custom_environ)
{
	int					idx;
	int					inside_sing_quotes;
	int					newline;
	struct s_echo_args	args;

	idx = 0;
	inside_sing_quotes = 0;
	newline = 1;
	echo_n_handler(buf, &idx, &newline);
	args.buf = buf;
	args.custom_environ = custom_environ;
	args.idx = &idx;
	args.inside_sing_quotes = &inside_sing_quotes;
	args.newline = newline;
	process_echo_command(&args);
	g_exit_code = 0;
}

char	*export_validator(char *buf)
{
	t_ValidationArgs	*args;
	char				*result;

	args = (t_ValidationArgs *)malloc(sizeof(t_ValidationArgs));
	args->quote_type = (char *)malloc(sizeof(char) * 2);
	if (!args || !args->quote_type)
	{
		perror("Memory allocation failed");
		g_exit_code = 1;
		exit(1);
	}
	args->has_equal_sign = false;
	args->inside_quotes = 0;
	args->quote_type[0] = 0;
	result = validate_buffer(buf, args);
	free(args);
	return (result);
}

int	update_env_var(char ***custom_environ, char *new_buf, int idx)
{
	char	**splitted_environ;
	char	**splitted_new_buf;
	int		is_updated;

	splitted_environ = ft_split((*custom_environ)[idx], '=');
	splitted_new_buf = ft_split(new_buf, '=');
	is_updated = 0;
	if (ft_strcmp(splitted_environ[0], splitted_new_buf[0]) == 0)
	{
		free((*custom_environ)[idx]);
		(*custom_environ)[idx] = ft_strdup(new_buf);
		is_updated = 1;
	}
	ft_free_char_arr(splitted_environ);
	ft_free_char_arr(splitted_new_buf);
	return (is_updated);
}

void	add_new_env_var(char ***custom_environ, char *new_buf, int idx)
{
	(*custom_environ) = realloc((*custom_environ), (idx + 2) * sizeof(char *));
	(*custom_environ)[idx] = ft_strdup(new_buf);
	(*custom_environ)[idx + 1] = NULL;
}

char	**create_unset_arr(char *buf, char **custom_environ)
{
	char	*token;
	char	**to_delete;
	int		idx;

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
