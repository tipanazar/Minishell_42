#include "../../minishell.h"

int calculate_buf_if(int *i, int *in_double_quotes, int *in_single_quotes,
					 char quote_type, char *arg)
{
	if (arg[(*i)] == '\"' && !(*in_single_quotes))
		(*in_double_quotes) = !(*in_double_quotes);
	if (arg[(*i)] == '\'' && !(*in_double_quotes))
		(*in_single_quotes) = !(*in_single_quotes);
	if (arg[(*i)] == quote_type && (!(*in_double_quotes) || quote_type == '\"') && (!(*in_single_quotes) || quote_type == '\''))
	{
		(*i)++;
		return (1);
	}
	return (0);
}

void calculate_buf_change_nums(int *i, int *size, int *in_double_quotes,
							   int *in_single_quotes, int flag)
{
	if (flag == 1)
	{
		(*in_double_quotes) = 0;
		(*in_single_quotes) = 0;
		(*i) = 0;
		(*size) = 0;
	}
	else if (flag == 2)
	{
		(*size)++;
		(*i)++;
	}
}

int calculate_buf_var_code_error(char **arg, int *i, int *size)
{
	if (ft_strncmp((*arg) + (*i), "$?", 2) == 0)
	{
		(*size) += 3;
		(*i) += 2;
		return (0);
	}
	return (1);
}

char *handle_env_var(char *arg, int *i, int *memory_allocated, char **envp)
{
	char var_name[256];
	int k;
	char *env_value;

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
	env_value = custom_getenv(var_name, envp, false);
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

void calculate_buf_var_val(char **var_value, int *size, int memory_allocated)
{
	if ((*var_value) != NULL)
	{
		(*size) += ft_strlen((*var_value));
		if (memory_allocated)
			free((*var_value));
	}
}

int calculate_buffer_size(char *arg, char quote_type, bool in_quotes,
						  char **envp)
{
	char *var_value = NULL;
	int i = 0;
	int size = 0;
	int memory_allocated = 0;
	int in_double_quotes = 0;
	int in_single_quotes = 0;
	bool in_q = in_quotes;
	// calculate_buf_change_nums(&i, &size, &in_double_quotes, &in_single_quotes, 1);
	while (arg[i] != '\0')
	{
		if (calculate_buf_if(&i, &in_double_quotes, &in_single_quotes, quote_type, arg))
			in_q = !in_q;
		else if (arg[i] == '$' && ((!in_q && quote_type == '\'') || (in_q && quote_type == '\"')) && calculate_buf_var_code_error(&arg, &i, &size))
		{
			var_value = handle_env_var(arg, &i, &memory_allocated, envp);
			calculate_buf_var_val(&var_value, &size, memory_allocated);
		}
		else
		{
			size++;
			i++;
		}
			// calculate_buf_change_nums(&i, &size, &in_double_quotes, &in_single_quotes, 2);
	}
	return (size + 1);
}

void replace_env_vars_set(int *i, int *j, int *in_double_quotes)
{
	(*in_double_quotes) = 0;
	(*i) = 0;
	(*j) = 0;
}

int replace_env_vars_exit_code(int *is_itoa, char *arg, int *i,
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

int re_e_v_ch_em(char *var_value, char **result, int *j)
{
	if (var_value != NULL)
	{
		ft_strncpy((*result) + (*j), var_value, ft_strlen(var_value));
		(*j) += ft_strlen(var_value);
		return (1);
	}
	return (0);
}

char *replace_env_vars_return(char **result, int i)
{
	(*result)[i] = '\0';
	return ((*result));
}

void quotes_env_errors_set(int *i, int *j, int *in_double_quotes, int *in_single_quotes)
{
	(*in_double_quotes) = 0;
	(*in_single_quotes) = 0;
	(*i) = 0;
	(*j) = 0;
}

int quotes_env_errors_exit_code(int *is_itoa, char *arg, int *i,
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

char *quotes_env_errors_return(char **result, int i)
{
	(*result)[i] = '\0';
	return ((*result));
}

char *quotes_env_errors(char *arg, char q_ty, int in_q, char **envp)
{
	char *var_val;
	char *result;
	int i[2];
	int ch_va[4];
	result = (char *)malloc(calculate_buffer_size(arg, q_ty, false, envp) + 1);
	// result = malloc(6);
	if (!result)
		return (NULL);
	quotes_env_errors_set(&i[0], &i[1], &ch_va[2], &ch_va[3]);
	while (arg[i[0]] != '\0')
	{
		if (calculate_buf_if(&i[0], &ch_va[2], &ch_va[3], q_ty, arg))
			in_q = !in_q;
		else if (arg[i[0]] == '$' && ((!in_q && q_ty == '\'') || (in_q && q_ty == '\"')))
		{
			if (quotes_env_errors_exit_code(&ch_va[0], arg, &i[0], &var_val))
				var_val = handle_env_var(arg, &i[0], &ch_va[1], envp);
			if (re_e_v_ch_em(var_val, &result, &i[1]) && (ch_va[0] || ch_va[1]))
				free(var_val);
		}
		else
			result[i[1]++] = arg[i[0]++];
	}
	return (quotes_env_errors_return(&result, i[1]));
}

char  *process_quotes(char *arg, char quote1, char quote2, char **envp)
{
  char *processed_arg;
  char *temp;

  processed_arg = quotes_env_errors(arg, quote1, false, envp);
  temp = processed_arg;
  processed_arg = quotes_env_errors(processed_arg, quote2, false, envp);

  if (temp != arg && temp != processed_arg)
    free(temp);

  return (processed_arg);
}

char *handle_all_quotes(char *arg, char **envp)
{
	char *single_quote_ptr;
	char *double_quote_ptr;

	single_quote_ptr = ft_strchr(arg, '\'');
	double_quote_ptr = ft_strchr(arg, '\"');
	if (double_quote_ptr == NULL || (single_quote_ptr != NULL && single_quote_ptr < double_quote_ptr))
		return (process_quotes(arg, '\"', '\'', envp));
	else
		return (process_quotes(arg, '\'', '\"', envp));
}

char *parseexec_arg_process(char *q, char *eq, char **envp)
{
	char *arg;
	char *processed_arg;

	arg = mkcopy((q), (eq));
	processed_arg = handle_all_quotes(arg, (envp));
	free(arg);
	return (processed_arg);
}