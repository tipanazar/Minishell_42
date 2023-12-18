#include "../../minishell.h"

void	handle_child_process(struct s_pipecmd *pcmd, int fd_pipe[2], char **env)
{
	close(fd_pipe[0]);
	dup2(fd_pipe[1], STDOUT_FILENO);
	close(fd_pipe[1]);
	runcmd(pcmd->left, env);
	free_cmd((struct s_cmd *)pcmd);
	ft_free_char_arr(env);
	exit(g_exit_code);
}

void	handle_parent_process(struct s_pipecmd *pcmd, int fd_pipe[2], int p_id,
		char **env)
{
	int	status;

	close(fd_pipe[1]);
	dup2(fd_pipe[0], STDIN_FILENO);
	close(fd_pipe[0]);
	waitpid(p_id, &status, 0);
	if (WIFEXITED(status))
		g_exit_code = WEXITSTATUS(status);
	runcmd(pcmd->right, env);
}

void	create_pipe_process(struct s_pipecmd *pcmd, int fd_pipe[2], char **env)
{
	int	p_id;

	p_id = fork();
	if (p_id < 0)
	{
		perror("fork");
		exit(1);
	}
	else if (p_id == 0)
		handle_child_process(pcmd, fd_pipe, env);
	else
		handle_parent_process(pcmd, fd_pipe, p_id, env);
}

void	pipe_command(struct s_pipecmd *pcmd, char **env)
{
	int	fd_pipe[2];

	if (pipe(fd_pipe) < 0)
	{
		perror("pipe");
		exit(1);
	}
	create_pipe_process(pcmd, fd_pipe, env);
}