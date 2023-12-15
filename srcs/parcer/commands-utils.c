#include "../../minishell.h"

int	getcmd(char *buf, int nbuf)
{
	if (isatty(0))
		write(2, "minishell# ", 11);
	ft_memset(buf, 0, nbuf);
	ft_fgets(buf, nbuf, stdin);
	if (buf[0] == 0)
		return (-1);
	return (0);
}

struct s_cmd	*execcmd(void)
{
	struct s_execcmd	*cmd;

	cmd = malloc(sizeof(*cmd));
	if (cmd == NULL)
	{
		perror("malloc");
		return (NULL);
	}
	ft_memset(cmd, 0, sizeof(*cmd));
	cmd->type = ' ';
	return ((struct s_cmd *)cmd);
}

struct s_cmd	*redircmd(struct s_cmd *subcmd, char *file, int type)
{
	struct s_redircmd	*cmd;

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
	else if (type == '>')
	{
		cmd->mode = O_WRONLY | O_CREAT | O_TRUNC;
		cmd->fd = 1;
	}
	else if (type == '+')
	{
		cmd->mode = O_WRONLY | O_CREAT | O_APPEND;
		cmd->fd = 1;
	}
	else if (type == '-')
	{
		cmd->mode = O_RDONLY;
		cmd->fd = 0;
	}
	return ((struct s_cmd *)cmd);
}

struct s_cmd	*pipecmd(struct s_cmd *left, struct s_cmd *right)
{
	struct s_pipecmd	*cmd;

	cmd = malloc(sizeof(*cmd));
	ft_memset(cmd, 0, sizeof(*cmd));
	cmd->type = '|';
	cmd->left = left;
	cmd->right = right;
	return ((struct s_cmd *)cmd);
}

char	*find_command_in_path(char *command)
{
	char		*PATH;
	char		*path;
	static char	abs_path[512];
	char		*temp_PATH;

	if (command[0] == '/' || ft_strchr(command, '/'))
	{
		if (access(command, X_OK) == 0)
			return (ft_strdup(command));
		return (NULL);
	}
	PATH = getenv("PATH");
	if (!PATH) // Check if PATH is NULL
		return (NULL);
	temp_PATH = ft_strdup(PATH);
	path = ft_strtok(temp_PATH, ":");
	while (path != NULL)
	{
		snprintf(abs_path, sizeof(abs_path), "%s/%s", path, command);
		if (access(abs_path, X_OK) == 0)
		{
			free(temp_PATH);
			return (abs_path);
		}
		path = ft_strtok(NULL, ":");
	}
	free(temp_PATH);
	return (NULL);
}