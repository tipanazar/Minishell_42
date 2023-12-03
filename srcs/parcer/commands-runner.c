#include "../../minishell.h"

int	double_redirect_left(struct s_redircmd *rcmd)
{
	char	buffer[1024];
	int		pipefd[2];
	size_t	delimiter_length;
	size_t	read_len;

	if (rcmd == NULL || rcmd->file == NULL)
	{
		fprintf(stderr, "Invalid command\n");
		return (-1);
	}
	if (pipe(pipefd) == -1)
	{
		perror("pipe");
		return (-1);
	}
	delimiter_length = (ssize_t)ft_strlen(rcmd->file);
	while (1)
	{
		fprintf(stderr, ">");
		read_len = read(STDIN_FILENO, buffer, sizeof(buffer) - 1);
		if (read_len <= 0)
			break ;
		buffer[read_len] = '\0';
		if (ft_strncmp(buffer, rcmd->file, (size_t)delimiter_length) == 0 &&
			(read_len == delimiter_length || buffer[delimiter_length] == '\n'))
			break ;
		write(pipefd[1], buffer, (size_t)read_len);
	}
	close(pipefd[1]);
	dup2(pipefd[0], STDIN_FILENO);
	close(pipefd[0]);
	return (1);
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

int	exec_cmd(struct s_execcmd *ecmd, char **environ)
{
	char	*abs_path;

	if (ecmd->argv[0] == 0)
		exit(1);
	if (builtins(concat_args(ecmd->argv), environ))
		exit(0);
	abs_path = find_command_in_path(ecmd->argv[0]);
	if (abs_path && execve(abs_path, ecmd->argv, environ) == -1)
	{
		perror("execve");
		exit(1);
	}
	else
		perror("Command not found");
	return (1);
}

int	redirect_cmd(struct s_redircmd *rcmd, char **environ)
{
	int	fd_redirect;
	int	flags;
	int	pipe_read_end;

	if (rcmd->type == '>')
		flags = O_WRONLY | O_CREAT | O_TRUNC;
	else if (rcmd->type == '<')
		flags = O_RDONLY;
	else if (rcmd->type == '+')
		flags = O_WRONLY | O_CREAT | O_APPEND;
	else if (rcmd->type == '-')
	{
		pipe_read_end = double_redirect_left(rcmd);
		if (pipe_read_end < 0)
		{
			perror("double_redirect_left");
			return (-1);
		}
		runcmd(rcmd->cmd, environ);
		return (1);
	}
	else
		return (-1);
	fd_redirect = open(rcmd->file, flags, 0666);
	if (fd_redirect < 0)
	{
		perror("open");
		return (-1);
	}
	if (dup2(fd_redirect, rcmd->fd) < 0)
	{
		perror("dup2");
		close(fd_redirect);
		return (-1);
	}
	runcmd(rcmd->cmd, environ);
	close(fd_redirect);
	return (1);
}

int	pipe_cmd(struct s_pipecmd *pcmd, char **env)
{
	int	fd_pipe[2];

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

int	runcmd(struct s_cmd *cmd, char **env)
{
	char	type;

	if (cmd == 0)
		exit(1);
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
