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
		exit(-1);
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
			write(2, "missing file for redirection\n", 29);
			exit(-1);
		}
		if (tok == '<')
			cmd = redircmd(cmd, mkcopy(q, eq), '<');
		else if (tok == '>')
			cmd = redircmd(cmd, mkcopy(q, eq), '>');
		else if (tok == '+')
			cmd = redircmd(cmd, mkcopy(q, eq), '+');
		else if (tok == '-')
			cmd = redircmd(cmd, mkcopy(q, eq), '-');
	}
	return (cmd);
}

void	expand_argv_if_needed(struct s_execcmd *cmd)
{
	if (cmd->argc >= cmd->max_args)
	{
		cmd->max_args *= 2;
		cmd->argv = ft_realloc(cmd->argv, cmd->max_args * sizeof(char *));
	}
}

struct s_cmd	*process_token(struct s_cmd *ret, struct s_execcmd *cmd,
		int tok, char **ps, char *es, char *q, char *eq)
{
	if (tok == '\'' || tok == '\"')
		cmd->argv[cmd->argc] = mkcopy(q, eq);
	else if (tok != 'a')
	{
		write(2, "syntax error\n", 12);
		free(ret);
		exit(-1);
	}
	else
		cmd->argv[cmd->argc] = mkcopy(q, eq);
	cmd->argc++;
	expand_argv_if_needed(cmd);
	return (parseredirs(ret, ps, es));
}

struct s_cmd	*parseexec(char **ps, char *es)
{
	struct s_execcmd	*cmd;
	struct s_cmd		*ret;
	int					tok;
	char				*q;
	char				*eq;

	ret = execcmd();
	cmd = (struct s_execcmd *)ret;
	cmd->argc = 0;
	cmd->max_args = 10;
	cmd->argv = malloc(cmd->max_args * sizeof(char *));
	if (cmd->argv == NULL)
	{
		perror("malloc");
		exit(-1);
	}
	ret = parseredirs(ret, ps, es);
	while (!peek(ps, es, "|"))
	{
		tok = get_token(ps, es, &q, &eq);
		if (tok == 0)
			break ;
		ret = process_token(ret, cmd, tok, ps, es, q, eq);
	}
	cmd->argv[cmd->argc] = 0;
	return (ret);
}
