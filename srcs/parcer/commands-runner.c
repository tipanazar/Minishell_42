#include "../../minishell.h"

char *find_command_in_path(char *command)
{
	char *PATH;
	char *path;
	static char abs_path[512];
	char *temp_PATH;

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

int exec_cmd(struct s_execcmd *ecmd, char **custom_environ)
{
	char *abs_path;

	if (ecmd->argv[0] == 0)
		exit(1); // Change to indicate error
	if (builtins(concat_args(ecmd->argv), custom_environ))
		exit(0);
	abs_path = find_command_in_path(ecmd->argv[0]);
	if (abs_path && execve(abs_path, ecmd->argv, custom_environ) == -1)
	{
		perror("execve");
		exit(1); // Change to indicate error
	}
	else
		perror("Command not found");
	return (1);
}

int redirect_cmd(struct s_redircmd *rcmd, char **custom_environ)
{
	int fd_redirect;
	int flags;

	if (rcmd->type == '>')
		flags = O_WRONLY | O_CREAT | O_TRUNC;
	else if (rcmd->type == '<')
		flags = O_RDONLY;
	else if (rcmd->type == '+')
		flags = O_WRONLY | O_CREAT | O_APPEND;
	else
		return (-1);
	fd_redirect = open(rcmd->file, flags, 0666);
	if (fd_redirect < 0)
	{
		perror("open");
		exit(1); // Change to indicate error
	}
	if (dup2(fd_redirect, rcmd->fd) < 0)
	{
		perror("dup2");
		close(fd_redirect);
		exit(1); // Change to indicate error
	}
	runcmd(rcmd->cmd, custom_environ);
	close(fd_redirect);
	return (1);
}

int pipe_cmd(struct s_pipecmd *pcmd, char **env)
{
	int fd_pipe[2];

	int p_id_left, p_id_right;
	int status_left, status_right;
	if (pipe(fd_pipe) < 0)
	{
		perror("pipe");
		return (-1);
	}
	p_id_left = fork();
	if (p_id_left < 0)
	{
		perror("fork");
		return (-1);
	}
	else if (p_id_left == 0)
	{
		close(fd_pipe[0]);
		dup2(fd_pipe[1], STDOUT_FILENO);
		close(fd_pipe[1]);
		runcmd(pcmd->left, env);
		exit(0);
	}
	p_id_right = fork();
	if (p_id_right < 0)
	{
		perror("fork");
		return (-1);
	}
	else if (p_id_right == 0)
	{
		close(fd_pipe[1]);
		dup2(fd_pipe[0], STDIN_FILENO);
		close(fd_pipe[0]);
		runcmd(pcmd->right, env);
		exit(0);
	}
	close(fd_pipe[0]);
	close(fd_pipe[1]);
	waitpid(p_id_left, &status_left, 0);
	waitpid(p_id_right, &status_right, 0);
	return (1);
}

int runcmd(struct s_cmd *cmd, char **env)
{
	char type;

	if (cmd == 0)
		exit(1); // Change to indicate error
	type = cmd->type;
	if (type == ' ')
		return (exec_cmd((struct s_execcmd *)cmd, env));
	else if (type == '>' || type == '<' || type == '+' || type == '-')
		return (redirect_cmd((struct s_redircmd *)cmd, env));
	else if (type == '|')
		return (pipe_cmd((struct s_pipecmd *)cmd, env));
	else
		ft_printf("unknown runcmd\n");
	return (1);
}
