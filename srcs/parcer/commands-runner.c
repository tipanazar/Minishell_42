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
			|| command->type == '+' || command->type == '%')
	{
		rcmd = (struct s_redircmd *)command;
		free_cmd(rcmd->cmd);
		free(rcmd->file);
	}
	free(command);
}

void	create_pipe_process(struct s_pipecmd *pcmd, int fd_pipe[2], char **env)
{
	int		p_id;
	int		status;
	char	**env2;

	env2 = env;
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
		free_cmd((struct s_cmd *)pcmd);
		ft_free_char_arr(env);
		exit(0);
	}
	else
	{
		close(fd_pipe[1]);
		dup2(fd_pipe[0], STDIN_FILENO);
		close(fd_pipe[0]);
		waitpid(p_id, &status, 0);
		runcmd(pcmd->right, env2);
	}
}

void	setup_pipe(int fd_pipe[2])
{
	if (pipe(fd_pipe) < 0)
	{
		write(2, "pipe has failed\n", 14);
		exit(0);
	}
}

void	pipe_command(struct s_pipecmd *pcmd, char **env)
{
	int	fd_pipe[2];

	setup_pipe(fd_pipe);
	create_pipe_process(pcmd, fd_pipe, env);
}

int	double_redirect_left(struct s_redircmd *rcmd)
{
	char	buffer[1024];
	int		pipefd[2];
	size_t	delimiter_length;
	ssize_t	read_len;

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
			break ;
		buffer[read_len] = '\0';
		if (ft_strncmp(buffer, rcmd->file, delimiter_length) == 0 &&
			(buffer[delimiter_length] == '\n'
					|| buffer[delimiter_length] == '\0'))
			break ;
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

int	redirect_cmd(struct s_redircmd *rcmd, char **custom_environ)
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
		runcmd(rcmd->cmd, custom_environ);
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
	runcmd(rcmd->cmd, custom_environ);
	close(fd_redirect);
	return (1);
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
