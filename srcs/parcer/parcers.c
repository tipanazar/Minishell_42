#include "../../minishell.h"

struct s_cmd	*parsecmd(char *s)
{
	char			*es;
	struct s_cmd	*cmd;

	es = s + ft_strlen(s);
	cmd = parseline(&s, es);
	peek(&s, es, "");
	if (s != es)
	{
		write(2, "leftovers: %s\n", 14);
		exit(-1);
	}
	return (cmd);
}

struct s_cmd	*parseline(char **ps, char *es)
{
	struct s_cmd	*cmd;

	cmd = parsepipe(ps, es);
	return (cmd);
}

struct s_cmd	*parsepipe(char **ps, char *es)
{
	struct s_cmd	*cmd;

	cmd = parseexec(ps, es);
	if (peek(ps, es, "|"))
	{
		gettoken(ps, es, 0, 0);
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
		tok = gettoken(ps, es, 0, 0);
		if (gettoken(ps, es, &q, &eq) != 'a')
		{
			write(2, "missing file for redirection\n", 28);
			exit(-1);
		}
		if (tok == '<')
		{
			cmd = redircmd(cmd, mkcopy(q, eq), '<');
		}
		else if (tok == '>')
		{
			cmd = redircmd(cmd, mkcopy(q, eq), '>');
		}
	}
	return (cmd);
}

struct s_cmd	*parseexec(char **ps, char *es)
{
	struct s_execcmd	*cmd;
	struct s_cmd		*ret;
	char				*q;
	char				*eq;
	int					tok;
	int					argc;

	ret = execcmd();
	cmd = (struct s_execcmd *)ret;
	argc = 0;
	ret = parseredirs(ret, ps, es);
	while (!peek(ps, es, "|"))
	{
		if ((tok = gettoken(ps, es, &q, &eq)) == 0)
			break ;
		if (tok != 'a')
		{
			write(2, "syntax error\n", 12);
			exit(-1);
		}
		cmd->argv[argc] = mkcopy(q, eq);
		argc++;
		if (argc >= MAXARGS)
		{
			write(2, "too many args\n", 14);
			exit(-1);
		}
		ret = parseredirs(ret, ps, es);
	}
	cmd->argv[argc] = 0;
	return (ret);
}
