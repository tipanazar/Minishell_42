#include "../../minishell.h"

void	setup_pipe(int pipefd[])
{
	if (pipe(pipefd) == -1)
	{
		perror("pipe");
		exit(EXIT_FAILURE);
	}
}

pid_t	execute_left_command(struct s_pipecmd *pcmd, char ***env, int pipefd[])
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
		// dprintf(2, "P_id : %d \n", left_pid);
		close(pipefd[0]);
		dup2(pipefd[1], STDOUT_FILENO);
		close(pipefd[1]);
		dprintf(2, "\nLEFT type : %d\n\n", pcmd->left->type);
		runcmd(pcmd->left, env);
		dprintf(2, "\nLEFT pipe : %s\n\n", ((struct s_execcmd *)pcmd->left)->argv[0]);
		ft_free_char_arr(*env);
		free_cmd((struct s_cmd *)pcmd);
		exit(EXIT_SUCCESS);
	}
	return (left_pid);
}

pid_t	execute_right_command(struct s_pipecmd *pcmd, char ***env, int pipefd[])
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
		// dprintf(2, "P_id : %d \n", right_pid);
		close(pipefd[1]);
		dup2(pipefd[0], STDIN_FILENO);
		close(pipefd[0]);
		dprintf(2, "\nRIGHT type : %d\n\n", pcmd->right->type);
		runcmd(pcmd->right, env);
		dprintf(2, "\nRIGHT pipe : %s\n\n", ((struct s_execcmd *)pcmd->right)->argv[0]);
		ft_free_char_arr(*env);
		free_cmd((struct s_cmd *)pcmd);
		exit(EXIT_SUCCESS);
	}
	return (right_pid);
}

void	pipe_command(struct s_pipecmd *pcmd, char ***env)
{
	int		pipefd[2];
	pid_t	left_pid;
	pid_t	right_pid;

	setup_pipe(pipefd);
	if(pcmd->left)
		left_pid = execute_left_command(pcmd, env, pipefd);
	if (pcmd->left->type == '-')
		waitpid(left_pid, NULL, 0);
	right_pid = execute_right_command(pcmd, env, pipefd);
	if (pcmd->right->type == '-')
		waitpid(right_pid, NULL, 0);
	close(pipefd[0]);
	close(pipefd[1]);
	waitpid(right_pid, NULL, 0);
	waitpid(left_pid, NULL, 0);
}