#include "../../minishell.h"

int	getcmd(char *buf, int nbuf)
{
	if (isatty(fileno(stdin)))
		write(2, "minishell> ", 11);
	ft_memset(buf, 0, nbuf);
	fgets(buf, nbuf, stdin);
	if (buf[0] == 0)
		return (-1);
	return (0);
}

struct cmd	*execcmd(void)
{
	struct execcmd	*cmd;

	cmd = malloc(sizeof(*cmd));
	ft_memset(cmd, 0, sizeof(*cmd));
	cmd->type = ' ';
	return ((struct cmd *)cmd);
}

struct cmd	*redircmd(struct cmd *subcmd, char *file, int type)
{
	struct redircmd	*cmd;

	cmd = malloc(sizeof(*cmd));
	ft_memset(cmd, 0, sizeof(*cmd));
	cmd->type = type;
	cmd->cmd = subcmd;
	cmd->file = file;
	if (type == '<')
	{
		cmd->mode = O_RDONLY;
		cmd->fd = 0;
	}
	else
	{
		cmd->mode = O_WRONLY | O_CREAT | O_TRUNC;
		cmd->fd = 1;
	}
	return ((struct cmd *)cmd);
}

struct cmd	*pipecmd(struct cmd *left, struct cmd *right)
{
	struct pipecmd	*cmd;

	cmd = malloc(sizeof(*cmd));
	ft_memset(cmd, 0, sizeof(*cmd));
	cmd->type = '|';
	cmd->left = left;
	cmd->right = right;
	return ((struct cmd *)cmd);
}
