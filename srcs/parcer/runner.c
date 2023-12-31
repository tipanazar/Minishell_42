#include "../../minishell.h"

int	check_error(char *cmd)
{
	if (errno == EACCES)
	{
		write(2, cmd, strlen(cmd));
		write(2, ": permission denied\n", 20);
		return (126);
	}
	else if (errno == ENOENT)
	{
		write(2, cmd, strlen(cmd));
		write(2, ": command not found\n", 20);
		return (127);
	}
	else
	{
		perror(cmd);
		return (127);
	}
}

void	free_exec_cmd(struct s_execcmd *ecmd)
{
			ft_free_char_arr(ecmd->argv);
}

void	free_cmd(struct s_cmd *command)
{
	struct s_pipecmd	*pcmd;
	struct s_redircmd	*rcmd;

	if (!command)
		return ;
	if (command->type == ' ')
		free_exec_cmd((struct s_execcmd *)command);
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

void	execute_command1(struct s_execcmd *ecmd, char **custom_environ)
{
	pid_t	pid;
	int		status;
	char	*full_path;

	pid = fork();
	if (pid < 0)
	{
		perror("fork");
		exit(1);
	}
	else if (pid == 0)
	{
		full_path = find_command_in_path(ecmd->argv[0], custom_environ);
		if (full_path)
		{
			execve(full_path, ecmd->argv, custom_environ);
			if (errno)
				g_exit_code = check_error(ecmd->argv[0]);
		}
		else
		{
			execve(ecmd->argv[0], ecmd->argv, custom_environ);
			if (errno)
				g_exit_code = check_error(ecmd->argv[0]);
		}
		// free_cmd((struct s_cmd*)ecmd);
		// ft_free_char_arr(custom_environ);
		exit(g_exit_code);
	}
	else
	{
		waitpid(pid, &status, 0);
		if (WIFEXITED(status))
			g_exit_code = WEXITSTATUS(status);
	}
}

int	exec_cmd(struct s_cmd *cmd, char **custom_environ)
{
	struct s_execcmd	*ecmd;

	ecmd = (struct s_execcmd *)cmd;
	if (ecmd->argv[0] != NULL)
	{
		if (ecmd->argv[0] == 0)
			exit(0);
		if (builtins(ecmd->argv, custom_environ))
			return (g_exit_code);
		execute_command1(ecmd, custom_environ);
		// ft_printf("Free...\n");
		// ft_free_char_arr(ecmd->argv);
		// free(ecmd);
	}
	return (g_exit_code);
}

int	runcmd(struct s_cmd *cmd, char **env)
{
	char	type;

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
