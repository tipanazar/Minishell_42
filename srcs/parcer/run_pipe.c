#include "../../minishell.h"

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
	int fd_pipe[2];

	setup_pipe(fd_pipe);
	create_pipe_process(pcmd, fd_pipe, env);
}