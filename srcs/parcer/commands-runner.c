#include "../../minishell.h"

void create_pipe_process(struct s_pipecmd *pcmd, int fd_pipe[2], char **env)
{
	int p_id;
	int status;

	p_id = fork();
	if (p_id < 0)
	{
		write(2, "fork has failed\n", 16);
		exit(1);
	}
	else if (p_id == 0)
	{
		close(fd_pipe[0]);
		dup2(fd_pipe[1], STDOUT_FILENO);
		close(fd_pipe[1]);
		runcmd(pcmd->left, env);
		exit(0);
	}
	else
	{
		close(fd_pipe[1]);
		dup2(fd_pipe[0], STDIN_FILENO);
		close(fd_pipe[0]);
		waitpid(p_id, &status, 0);
		runcmd(pcmd->right, env);
	}
}

void setup_pipe(int fd_pipe[2])
{
	if (pipe(fd_pipe) < 0)
	{
		write(2, "pipe has failed\n", 14);
		exit(0);
	}
}

void pipe_command(struct s_pipecmd *pcmd, char **env)
{
	int fd_pipe[2];

	setup_pipe(fd_pipe);
	create_pipe_process(pcmd, fd_pipe, env);
}

int double_redirect_left(struct s_redircmd *rcmd)
{
	char buffer[1024];
	int pipefd[2];
	size_t delimiter_length;
	ssize_t read_len;

	if (rcmd == NULL || rcmd->file == NULL)
	{
		write(STDERR_FILENO, "Invalid command\n", 16);
		return (0);
	}
	if (pipe(pipefd) == -1)
	{
		perror("pipe");
		return (0);
	}
	delimiter_length = ft_strlen(rcmd->file);
	while (1)
	{
		write(STDERR_FILENO, ">", 1);
		read_len = read(STDIN_FILENO, buffer, sizeof(buffer) - 1);
		if (read_len <= 0)
			break;
		buffer[read_len] = '\0';
		if (ft_strncmp(buffer, rcmd->file, delimiter_length) == 0 &&
			(buffer[delimiter_length] == '\n' || buffer[delimiter_length] == '\0'))
			break;
		write(pipefd[1], buffer, read_len);
	}
	close(pipefd[1]);
	if (dup2(pipefd[0], STDIN_FILENO) == -1)
	{
		perror("dup2");
		close(pipefd[0]);
		return (0);
	}
	close(pipefd[0]);
	return (1);
}

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
	char *buf = concat_args(ecmd->argv);

	if (ecmd->argv[0] == 0)
		exit(1);
	if (builtins(buf, custom_environ))
	{
		ft_free_char_arr(ecmd->argv);
		free(ecmd);
		ft_free_char_arr(custom_environ);
		free(buf);
		exit(0);
	}
	free(buf);
	abs_path = find_command_in_path(ecmd->argv[0]);
	if (abs_path && execve(abs_path, ecmd->argv, custom_environ) == -1)
	{
		perror("execve");
		exit(1);
	}
	else
	{
		perror("Command not found");
		ft_free_char_arr(ecmd->argv);
		free(ecmd);
	}
	return (1);
}

int redirect_cmd(struct s_redircmd *rcmd, char **custom_environ)
{
	int fd_redirect;
	int flags;
	int pipe_read_end;

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
		runcmd(rcmd->cmd, custom_environ);
		return (1);
	}
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
	free(rcmd->file);
	runcmd(rcmd->cmd, custom_environ);
	return (1);
}

int runcmd(struct s_cmd *cmd, char **env)
{
	char type;
	
	if (cmd == 0)
		exit(1);
	type = cmd->type;
	if (type == ' ')
		exec_cmd((struct s_execcmd *)cmd, env);
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
