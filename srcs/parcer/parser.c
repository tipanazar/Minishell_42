#include "../../minishell.h"

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

struct s_cmd	*handle_parseexec_loop(struct s_execcmd *cmd, char **ps,
		char *es)
{
	struct s_process_token_args	args;
	int							tok;
	struct s_cmd				*ret;
	char						*q;
	char						*eq;

	while (!peek(ps, es, "|"))
	{
		tok = get_token(ps, es, &q, &eq);
		if (tok == 0)
			break ;
		args = (struct s_process_token_args){(struct s_cmd *)cmd, cmd, tok, ps,
			es, q, eq};
		ret = process_token(&args);
	}
	(void)ret;
	cmd->argv[cmd->argc] = 0;
	return ((struct s_cmd *)cmd);
}

void	expand_argv_if_needed(struct s_execcmd *cmd)
{
	if (cmd->argc >= cmd->max_args)
	{
		cmd->max_args *= 2;
		cmd->argv = ft_realloc(cmd->argv, cmd->max_args * sizeof(char *));
	}
}

struct s_cmd	*process_token(struct s_process_token_args *args)
{
	if (args->tok == '\'' || args->tok == '\"')
		args->cmd->argv[args->cmd->argc] = mkcopy(args->q, args->eq);
	else if (args->tok != 'a')
	{
		write(2, "syntax error\n", 12);
		free(args->ret);
		exit(-1);
	}
	else
		args->cmd->argv[args->cmd->argc] = mkcopy(args->q, args->eq);
	args->cmd->argc++;
	expand_argv_if_needed(args->cmd);
	return (parseredirs(args->ret, args->ps, args->es));
}

struct s_cmd	*parseexec(char **ps, char *es)
{
	struct s_execcmd	*cmd;
	struct s_cmd		*ret;

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
	return (handle_parseexec_loop(cmd, ps, es));
}
