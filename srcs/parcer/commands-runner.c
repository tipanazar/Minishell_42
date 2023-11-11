#include "../../minishell.h"

extern char **environ;

void env(void)
{
	char **env;

	env = environ;
	while (*env)
	{
		printf("%s\n", *env);
		env++;
	}
}

char *find_command_in_path(char *command)
{
	char *PATH;
	char *path;
	static char abs_path[512];
	char *temp_PATH;

	builtins(command);
	if (command[0] == '/' || ft_strchr(command, '/'))
	{
		if (access(command, X_OK) == 0)
			return (ft_strdup(command));
		return (NULL);
	}
	PATH = getenv("PATH");
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

// void exec_cmd(struct s_execcmd *ecmd)
// {
// 	char *abs_path;

// 	if (ecmd->argv[0] == 0)
// 		exit(0);
// 	abs_path = find_command_in_path(ecmd->argv[0]);
// 	if (abs_path)
// 		execve(abs_path, ecmd->argv, environ);
// 	else
// 		perror("Command not found");
// }

// void execute_command(struct s_cmd *cmd)
// {
// 	struct s_execcmd *ecmd;
// 	char *full_path;

// 	ecmd = (struct s_execcmd *)cmd;
// 	if (ecmd->argv[0] == 0)
// 		exit(0);
// 	if (builtins(ecmd))
// 		return;
// 	full_path = find_in_path(ecmd->argv[0]);
// 	if (full_path)
// 	{
// 		execve(full_path, ecmd->argv, ecmd->envp);
// 		free(full_path);
// 	}
// 	else
// 	{
// 		execve(ecmd->argv[0], ecmd->argv, ecmd->envp);
// 	}
// }

// void exec_cmd(struct s_execcmd *ecmd)
// {
// 	char *abs_path;

// 	if (ecmd->argv[0] == 0)
// 		exit(0);
// 	abs_path = find_command_in_path(ecmd->argv[0]);
// 	if (abs_path)
// 		execve(abs_path, ecmd->argv, environ);
// 	else
// 		perror("Command not found");
// }

void exec_cmd(struct s_execcmd *ecmd)
{
	char *abs_path;

	if (ecmd->argv[0] == 0)
		exit(0);

	ft_print_char_arr(ecmd->argv);	

	abs_path = find_command_in_path(ecmd->argv[0]);
	if (builtins(concat_args(ecmd->argv)))
	{
		ft_printf("Return exec_cmd \n");
		return;
	}
	else if (abs_path)
		execve(abs_path, ecmd->argv, environ);
	else 
		execve(ecmd->argv[0], ecmd->argv, environ);
	ft_printf("WC go\n");
}

int redirect_cmd(struct s_redircmd *rcmd)
{
	int fd_redirect;
	int flags;

	if (rcmd->type == '>')
		flags = O_WRONLY | O_CREAT | O_TRUNC;
	else if (rcmd->type == '<')
		flags = O_RDONLY;
	else
		flags = O_WRONLY | O_CREAT | O_APPEND;
	fd_redirect = open(rcmd->file, flags, 0666);
	if (fd_redirect < 0)
	{
		perror("open");
		exit(0);
	}
	if (dup2(fd_redirect, rcmd->fd) < 0)
	{
		perror("dup2");
		exit(0);
	}
	runcmd(rcmd->cmd);
	close(fd_redirect);
	return (1);
}

int pipe_cmd(struct s_pipecmd *pcmd)
{
	int fd_pipe[2];
	int p_id;

	if (pipe(fd_pipe) < 0)
		exit(0);
	p_id = fork();
	if (p_id < 0)
		exit(0);
	else if (p_id == 0)
	{
		close(fd_pipe[1]);
		dup2(fd_pipe[0], STDIN_FILENO);
		runcmd(pcmd->right);
		close(fd_pipe[0]);
	}
	else
	{
		close(fd_pipe[0]);
		dup2(fd_pipe[1], STDOUT_FILENO);
		runcmd(pcmd->left);
		close(fd_pipe[1]);
		wait(&p_id);
	}
	return (1);
}

void runcmd(struct s_cmd *cmd)
{
	char type;

	if (cmd == 0)
		exit(0);
	type = cmd->type;
	if (type == ' ')
		exec_cmd((struct s_execcmd *)cmd);
	else if (type == '>' || type == '<')
		redirect_cmd((struct s_redircmd *)cmd);
	else if (type == '|')
		pipe_cmd((struct s_pipecmd *)cmd);
	else
		free(cmd);
}
