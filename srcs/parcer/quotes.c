#include "../../minishell.h"

int	calculate_buf_if(int *i, int *in_double_quotes, char quote_type, char *arg)
{
	if (arg[(*i)] == '\"')
		(*in_double_quotes) = !(*in_double_quotes);
	if (arg[(*i)] == quote_type && (!(*in_double_quotes) || quote_type == '\"'))
	{
		(*i)++;
		return (1);
	}
	return (0);
}

void	calculate_buf_change_nums(int *i, int *size, int *in_double_quotes,
		int flag)
{
	if (flag == 1)
	{
		(*in_double_quotes) = 0;
		(*i) = 0;
		(*size) = 0;
	}
	else if (flag == 2)
	{
		(*size)++;
		(*i)++;
	}
}

int	calculate_buf_var_code_error(char **arg, int *i, int *size)
{
	if (ft_strncmp((*arg) + (*i), "$?", 2) == 0)
	{
		(*size) += 3;
		(*i) += 2;
		return (0);
	}
	return (1);
}

char	*builtin_getenv(const char *var, char **envp)
{
	int	i;

	if (var == NULL)
	{
		return (NULL);
	}
	i = 0;
	while (envp[i])
	{
		if (!ft_strncmp(envp[i], var, ft_strlen(var))
			&& envp[i][ft_strlen(var)] == '=')
		{
			return (&envp[i][ft_strlen(var) + 1]);
		}
		i++;
	}
	return (NULL);
}

char	*handle_env_var(char *arg, int *i, int *memory_allocated, char **envp)
{
	char	var_name[256];
	int		k;
	char	*env_value;

	k = 0;
	(*i)++;
	while (ft_isalnum(arg[*i]) || arg[*i] == '_')
		var_name[k++] = arg[(*i)++];
	var_name[k] = '\0';
	if (var_name[0] == '\0')
	{
		*memory_allocated = 1;
		return (ft_strdup("$"));
	}
	env_value = builtin_getenv(var_name, envp);
	if (env_value != NULL)
	{
		*memory_allocated = 1;
		return (ft_strdup(env_value));
	}
	else
	{
		*memory_allocated = 1;
		return (NULL);
	}
}

void	calculate_buf_var_val(char **var_value, int *size, int memory_allocated)
{
	if ((*var_value) != NULL)
	{
		(*size) += ft_strlen((*var_value));
		if (memory_allocated)
			free((*var_value));
	}
}

int	calculate_buffer_size(char *arg, char quote_type, int in_quotes,
		char **envp)
{
	char	*var_value;
	int		i;
	int		size;
	int		memory_allocated;
	int		in_double_quotes;

	calculate_buf_change_nums(&i, &size, &in_double_quotes, 1);
	while (arg[i] != '\0')
	{
		if (calculate_buf_if(&i, &in_double_quotes, quote_type, arg))
			in_quotes = !in_quotes;
		else if (arg[i] == '$' && ((!in_quotes && quote_type == '\'')
						|| (in_quotes && quote_type == '\"'))
						&& calculate_buf_var_code_error(&arg, &i, &size))
		{
			var_value = handle_env_var(arg, &i, &memory_allocated, envp);
			calculate_buf_var_val(&var_value, &size, memory_allocated);
		}
		else
			calculate_buf_change_nums(&i, &size, &in_double_quotes, 2);
	}
	return (size + 1);
}

void	replace_env_vars_set(int *i, int *j, int *in_double_quotes)
{
	(*in_double_quotes) = 0;
	(*i) = 0;
	(*j) = 0;
}

int	replace_env_vars_exit_code(int *is_itoa, char *arg, int *i,
		char **var_value)
{
	(*is_itoa) = 0;
	if (ft_strncmp(arg + (*i), "$?", 2) == 0)
	{
		(*var_value) = ft_itoa(g_exit_code);
		(*i) += 2;
		(*is_itoa) = 1;
		return (0);
	}
	return (1);
}

int	re_e_v_ch_em(char *var_value, char **result, int *j)
{
	if (var_value != NULL)
	{
		ft_strncpy((*result) + (*j), var_value, ft_strlen(var_value));
		(*j) += ft_strlen(var_value);
		return (1);
	}
	return (0);
}

char	*replace_env_vars_return(char **result, int i)
{
	(*result)[i] = '\0';
	return ((*result));
}

char	*replace_env_vars(char *arg, char q_ty, int in_q, char **envp)
{
	char	*var_val;
	char	*result;
	int		i[2];
	int		ch_va[3];

	result = malloc((calculate_buffer_size(arg, q_ty, 0, envp)) + 1);
	if (!result)
	{
		return (NULL);
	}
	replace_env_vars_set(&i[0], &i[1], &ch_va[2]);
	while (arg[i[0]] != '\0')
	{
		if (calculate_buf_if(&i[0], &ch_va[2], q_ty, arg))
			in_q = !in_q;
		else if (arg[i[0]] == '$' && ((!in_q && q_ty == '\'') || (in_q
						&& q_ty == '\"')))
		{
			if (replace_env_vars_exit_code(&ch_va[0], arg, &i[0], &var_val))
				var_val = handle_env_var(arg, &i[0], &ch_va[1], envp);
			if (re_e_v_ch_em(var_val, &result, &i[1]) && (ch_va[0] || ch_va[1]))
				free(var_val);
		}
		else
			result[i[1]++] = arg[i[0]++];
	}
	result[i[1]] = '\0';
	return (result);
}

char	*handle_quotes(char *arg, char quote_type, char **envp)
{
	char	*result;

	result = replace_env_vars(arg, quote_type, 0, envp);
	return (result);
}

char	*parseexec_arg_process(char *q, char *eq, char **envp)
{
	char	*arg;
	char	*processed_arg_sing;
	char	*processed_arg_doub;

	arg = mkcopy(q, eq);
	processed_arg_sing = handle_quotes(arg, '\'', envp);
	processed_arg_doub = handle_quotes(processed_arg_sing, '\"', envp);
	free(arg);
	free(processed_arg_sing);
	return (processed_arg_doub);
}