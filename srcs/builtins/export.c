#include "../../minishell.h"

int update_env_var(char ***custom_environ, char *new_buf, int idx)
{
	char **splitted_environ;
	char **splitted_new_buf;
	int is_updated;

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

void upd_or_add_var(char ***custom_environ, char *buf)
{
	int idx = -1;
	while ((*custom_environ)[++idx])
		if (update_env_var(custom_environ, buf, idx))
			return;
	(*custom_environ) = realloc((*custom_environ), (idx + 2) * sizeof(char *));
	(*custom_environ)[idx] = ft_strdup(buf);
	(*custom_environ)[idx + 1] = NULL;
}

bool export_validator(char *buf, bool *stop_flag)
{
	bool has_equal_sign;

	has_equal_sign = false;
	int idx = -1;
	if (!ft_strchr(buf, '='))
		return true;
	while (buf[++idx])
	{
		if (buf[idx] == '=')
			has_equal_sign = true;
	}
	if ((buf[0] != '_' && !ft_isalpha(buf[0])) || !(has_equal_sign) )
	{
		ft_printf("-minishell: export: `%s': not a valid identifier\n", buf);
		*stop_flag = 1;
		return (true);
	}
	return false;
}

void export(char **buf_arr, char ***custom_environ, int argc)
{
	int f_idx = -1;
	bool stop_flag = 0;
	if(argc == 1)
	{
		while((*custom_environ)[++f_idx])
			ft_printf("declare -x %s\n", (*custom_environ)[f_idx]);
		return;
	}
	while (buf_arr[++f_idx])
		if (export_validator(buf_arr[f_idx], &stop_flag) == 0)
			upd_or_add_var(custom_environ, buf_arr[f_idx]);
	if (stop_flag)
		g_exit_code = 1;
	else
		g_exit_code = 0;
}
