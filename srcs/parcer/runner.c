#include "../../minishell.h"

void	free_cmd(struct s_cmd *command)
{
	struct s_pipecmd	*pcmd;
	struct s_execcmd	*ecmd;
	struct s_redircmd	*rcmd;
	int					i;

	i = 0;
	if (!command)
		return ;
	if (command->type == ' ')
	{
		ecmd = (struct s_execcmd *)command;
		while (ecmd->argv[i])
		{
			free(ecmd->argv[i]);
			i++;
		}
		free(ecmd->argv);
	}
	else if (command->type == '|')
	{
		pcmd = (struct s_pipecmd *)command;
		free_cmd(pcmd->left);
		free_cmd(pcmd->right);
	}
	else if (command->type == '>' || command->type == '<'
			|| command->type == '+' || command->type == '-')
	{
		rcmd = (struct s_redircmd *)command;
		free_cmd(rcmd->cmd);
		free(rcmd->file);
	}
	free(command);
}

int	exec_cmd(struct s_cmd *cmd, char **custom_environ)
{
	struct s_execcmd	*ecmd;
	char				*full_path;
	int					exit_code;
	char				*buf;

	exit_code = 0;
	ecmd = (struct s_execcmd *)cmd;
	if (ecmd->argv[0] == 0)
		exit(0);
	buf = concat_args(ecmd->argv);
	if (builtins(buf, custom_environ))
	{
		free(buf);
		return (exit_code);
	}
	full_path = find_command_in_path(ecmd->argv[0]);
	if (full_path)
	{
		execve(full_path, ecmd->argv, custom_environ);
		free(full_path);
	}
	else
		execve(ecmd->argv[0], ecmd->argv, custom_environ);
	perror("Command not found");
	free(buf);
	return (exit_code);
}

int	runcmd(struct s_cmd *cmd, char **env)
{
	char	type;

	if (cmd == 0)
		exit(1);
	type = cmd->type;
	if (type == ' ')
		exec_cmd(cmd, env);
	else if (type == '>' || type == '<' || type == '+' || type == '-')
		redirect_cmd((struct s_redircmd *)cmd, env);
	else if (type == '|')
		pipe_command((struct s_pipecmd *)cmd, env);
	else
	{
		ft_printf("unknown runcmd\n");
		exit(1);
	}
	return (1);
}
