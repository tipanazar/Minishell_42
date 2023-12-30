#include "../../minishell.h"

struct s_cmd	*parsecmd(char *s, char **custom_env)
{
	char			*es;
	struct s_cmd	*cmd;

	es = s + ft_strlen(s);
	cmd = parsepipe(&s, es, custom_env);
	peek(&s, es, "");
	if (s != es)
	{
		write(2, "leftovers: %s\n", 14);
		free(s);
		free(cmd);
		return (NULL);
	}
	return (cmd);
}


struct s_cmd	*parsepipe(char **ps, char *es, char **custom_env)
{
	struct s_cmd	*cmd;

	cmd = parseexec(ps, es, custom_env);
	if (peek(ps, es, "|"))
	{
		get_token(ps, es, 0, 0);
		cmd = pipecmd(cmd, parsepipe(ps, es, custom_env));
	}
	return (cmd);
}

struct s_cmd	*parseredirs(struct s_cmd *cmd, char **ps, char *es)
{
	int		tok;
	char	*q;
	char	*eq;

	while (peek(ps, es, "<>"))
	{
		tok = get_token(ps, es, 0, 0);
		if (tok == -1)
		{
			g_exit_code = 2;
			cmd->flag = false;
			return cmd;
		}
		if (get_token(ps, es, &q, &eq) != 'a')
		{
			write(2, "syntax error near unexpected token `newline'\n", 46);
			g_exit_code = 2;
			cmd->flag = 1;
			return (cmd);
		}
		if (tok == '<' || tok == '>' || tok == '+' || tok == '%')
			cmd = redircmd(cmd, mkcopy(q, eq), tok);
	}
	return (cmd);
}

void	parseexec_middleware(t_parseexec **parseexec_vars,
							struct s_execcmd **cmd, char **custom_env)
{
	if ((*parseexec_vars)->tok != 'a')
	{
		write(2, "syntax error\n", 13);
		return ;
	}
	else
		(*cmd)->argv[(*cmd)->argc] = parseexec_arg_process((*parseexec_vars)->q,
											(*parseexec_vars)->eq, custom_env);
	(*cmd)->argc++;
	if ((*cmd)->argc >= (*cmd)->max_args)
	{
		(*cmd)->max_args = (*cmd)->argc + 1;
		(*cmd)->argv = realloc((*cmd)->argv, (*cmd)->max_args
				* sizeof(char *));
	}
}

struct s_cmd	*parseexec(char **ps, char *es, char **custom_env)
{
	struct s_cmd		*ret;
	struct s_execcmd	*cmd;
	t_parseexec			*parseexec_vars;

	ret = execcmd();
	cmd = (struct s_execcmd *)ret;
	ret = parseredirs(ret, ps, es);
	cmd->argc = 0;
	cmd->max_args = 10;
	cmd->argv = malloc(cmd->max_args * sizeof(char *));
	parseexec_vars = malloc(sizeof(t_parseexec));
	while (!peek(ps, es, "|"))
	{
		parseexec_vars->tok = get_token(ps, es, &parseexec_vars->q,
				&parseexec_vars->eq);
		if (parseexec_vars->tok == 0)
			break ;
		parseexec_middleware(&parseexec_vars, &cmd, custom_env);
		ret = parseredirs(ret, ps, es);
	}
	free(parseexec_vars);
	cmd->argv[cmd->argc] = 0;
	return (ret);
}

// ! ! ! ! ! ! ! !

// int	ft_count_argc(char **ps, char *es)
// {
// 	char	*q;
// 	char	*eq;
// 	int		tok;
// 	int		argc;

// 	argc = 0;
// 	while (!peek(ps, es, "|") && !peek(ps, es, ">") && !peek(ps, es, "<")
// 		&& !peek(ps, es, ";") && !peek(ps, es, ">>") && !peek(ps, es, "<<"))
// 	{
// 		tok = get_token(ps, es, &q, &eq);
// 		if (tok == 0)
// 			break ;
// 		if (tok != 'a')
// 		{
// 			write(2, "syntax error\n", 12);
// 			return (argc);
// 		}
// 		argc++;
// 	}
// 	return (argc);
// }

// int	parseexec_count_argc(char **ps, char *es)
// {
// 	char	*ps_clone;
// 	char	*es_clone;

// 	ps_clone = *ps;
// 	es_clone = es;
// 	return (ft_count_argc(&ps_clone, es_clone));
// }

// void	handle_error(const char *error_message)
// {
// 	write(2, error_message, ft_strlen(error_message));
// 	exit(2);
// }

// int	parseexec_tok(char **q, char **eq, char **ps, char *es)
// {
// 	int	tok;

// 	tok = get_token(ps, es, q, eq);
// 	if (tok == 0)
// 		return (1);
// 	if (tok != 'a')
// 		handle_error("syntax\n");
// 	return (0);
// }

// struct s_cmd	*parseexec(char **ps, char *es, char **envp)
// {
// 	char				*q;
// 	char				*eq;
// 	int					argc;
// 	struct s_execcmd	*cmd;
// 	struct s_cmd		*ret;

// 	ret = execcmd();
// 	cmd = (struct s_execcmd *)ret;
// 	ret = parseredirs(ret, ps, es);
// 	argc = parseexec_count_argc(ps, es);
// 	cmd->argv = (char **)malloc(sizeof(char *) * (argc + 1));
// 	argc = 0;
// 	while (!peek(ps, es, "|"))
// 	{
// 		if (parseexec_tok(&q, &eq, ps, es))
// 			break ;
// 		cmd->argv[argc] = parseexec_arg_process(q, eq, envp);
// 		argc++;
// 		ret = parseredirs(ret, ps, es);
// 	}
// 	cmd->argv[argc] = 0;
// 	return (ret);
// }