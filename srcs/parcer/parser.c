#include "../../minishell.h"

struct s_cmd	*parsecmd(char *s)
{
	char			*es;
	struct s_cmd	*cmd;

	es = s + ft_strlen(s);
	cmd = parsepipe(&s, es);
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


struct s_cmd	*parsepipe(char **ps, char *es)
{
	struct s_cmd	*cmd;

	cmd = parseexec(ps, es);
	if (peek(ps, es, "|"))
	{
		get_token(ps, es, 0, 0);
		cmd = pipecmd(cmd, parsepipe(ps, es));
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
							struct s_execcmd **cmd)
{
	if ((*parseexec_vars)->tok == '\'' || (*parseexec_vars)->tok == '\"')
		(*cmd)->argv[(*cmd)->argc] = mkcopy((*parseexec_vars)->q,
											(*parseexec_vars)->eq);
	else if ((*parseexec_vars)->tok != 'a')
	{
		// write(2, "syntax error\n", 13);
		return ;
	}
	else
		(*cmd)->argv[(*cmd)->argc] = mkcopy((*parseexec_vars)->q,
											(*parseexec_vars)->eq);
	(*cmd)->argc++;
	if ((*cmd)->argc >= (*cmd)->max_args)
	{
		(*cmd)->max_args *= 2;
		(*cmd)->argv = ft_realloc((*cmd)->argv, (*cmd)->max_args
				* sizeof(char *));
	}
}

struct s_cmd	*parseexec(char **ps, char *es)
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
		parseexec_middleware(&parseexec_vars, &cmd);
		ret = parseredirs(ret, ps, es);
	}
	free(parseexec_vars);
	cmd->argv[cmd->argc] = 0;
	return (ret);
}
