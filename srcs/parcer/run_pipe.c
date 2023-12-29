#include "../../minishell.h"

void	setup_pipe(int pipefd[])
{
	if (pipe(pipefd) == -1)
	{
		perror("pipe");
		exit(EXIT_FAILURE);
	}
}

pid_t	execute_left_command(struct s_pipecmd *pcmd, char **env, int pipefd[])
{
	pid_t	left_pid;

	left_pid = fork();
	if (left_pid < 0)
	{
		perror("fork");
		exit(EXIT_FAILURE);
	}
	if (left_pid == 0)
	{
		close(pipefd[0]);
		dup2(pipefd[1], STDOUT_FILENO);
		close(pipefd[1]);
		exec_cmd(pcmd->left, env);
		free_cmd((struct s_cmd *)pcmd);
		ft_free_char_arr(env);
		exit(EXIT_SUCCESS);
	}
	return left_pid;
}

pid_t	execute_right_command(struct s_pipecmd *pcmd, char **env, int pipefd[])
{
	pid_t	right_pid;

	right_pid = fork();
	if (right_pid < 0)
	{
		perror("fork");
		exit(EXIT_FAILURE);
	}
	if (right_pid == 0)
	{
		close(pipefd[1]);
		dup2(pipefd[0], STDIN_FILENO);
		close(pipefd[0]);
		runcmd(pcmd->right, env);
		free_cmd((struct s_cmd *)pcmd);
		ft_free_char_arr(env);
		exit(EXIT_SUCCESS);
	}
	return right_pid;
}

void	close_pipes_and_wait(int pipefd[], pid_t left_pid, pid_t right_pid)
{
	close(pipefd[0]);
	close(pipefd[1]);
	waitpid(left_pid, NULL, 0);
	waitpid(right_pid, NULL, 0);
}

void	pipe_command(struct s_pipecmd *pcmd, char **env)
{
	int	pipefd[2];

	pid_t left_pid, right_pid;
	setup_pipe(pipefd);
	left_pid = execute_left_command(pcmd, env, pipefd);
	right_pid = execute_right_command(pcmd, env, pipefd);
	close_pipes_and_wait(pipefd, left_pid, right_pid);
}
