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
		dprintf(2, "Child process created with PID: %d\n", left_pid);
		close(pipefd[0]);
		dup2(pipefd[1], STDOUT_FILENO);
		close(pipefd[1]);
		runcmd(pcmd->left, env);
		free_cmd((struct s_cmd *)pcmd);
		ft_free_char_arr(env);
		exit(EXIT_SUCCESS);
	}
	return (left_pid);
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
		dprintf(2, "Child process created with PID: %d\n", right_pid);
		close(pipefd[1]);
		dup2(pipefd[0], STDIN_FILENO);
		close(pipefd[0]);
		runcmd(pcmd->right, env);
		free_cmd((struct s_cmd *)pcmd);
		ft_free_char_arr(env);
		exit(EXIT_SUCCESS);
	}
	return (right_pid);
}

void	pipe_command(struct s_pipecmd *pcmd, char **env)
{
	int		pipefd[2];
	pid_t	left_pid;
	pid_t	right_pid;

	setup_pipe(pipefd);
	left_pid = execute_left_command(pcmd, env, pipefd);
	if (pcmd->left->type == '-')
		waitpid(left_pid, NULL, 0);
	right_pid = execute_right_command(pcmd, env, pipefd);
	if (pcmd->right->type == '-')
		waitpid(right_pid, NULL, 0);
	close(pipefd[0]);
	close(pipefd[1]);
	waitpid(left_pid, NULL, 0);
	waitpid(right_pid, NULL, 0);
// void	pipe_command(struct s_pipecmd *pcmd, char **env)
// {
// 	// dprintf(2, "%S", pcmd->left);
// 	// dprintf(2, "%S", pcmd->right);
// 	runcmd(pcmd->left, env);
// 	runcmd(pcmd->right, env);
}

// void pipe_command(struct s_pipecmd *pcmd, char **env)
// {
//     int p[2]; // Array to hold the two file descriptors for the pipe
//     pid_t pid;

//     if (pipe(p) < 0) {
//         perror("pipe");
//         exit(1);
//     }

//     pid = fork();
//     if (pid < 0) {
//         perror("fork");
//         exit(1);
//     }

//     if (pid == 0) { // Child process
//         close(p[0]); // Close unused read end
//         dup2(p[1], STDOUT_FILENO); // Redirect standard output to pipe
//         close(p[1]); // Close write end of the pipe

//         runcmd(pcmd->left, env); // Execute left command
//         exit(0);
//     } else { // Parent process
//         close(p[1]); // Close unused write end
//         dup2(p[0], STDIN_FILENO); // Redirect standard input to pipe
//         close(p[0]); // Close read end of the pipe

//         waitpid(pid, NULL, 0); // Wait for child process to finish
//         runcmd(pcmd->right, env); // Execute right command
//     }
// }
