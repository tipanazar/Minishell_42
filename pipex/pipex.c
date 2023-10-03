#include "../minishell.h"

void error(void);
char **split(char *, char);
char *strjoin(char *, char *);
void execute(char *, char **);

void error(void)
{
    perror("\033[31mError");
    return ;
}

void parent_process(char **av, char **envp, int *fd)
{
    int fileout;

    fileout = open(av[4], O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if(fileout == -1)
        error();
    dup2(fd[0], STDIN_FILENO);
    dup2(fileout, STDOUT_FILENO);
    close(fd[1]);
    close(fileout);
    execute(av[3], envp);
}

char *find_path(char *cmd, char **envp)
{
    char **paths;
    char *path;
    int i;
    char *part_path;

    i = 0;
    while(ft_strnstr(envp[i], "PATH", 4) == 0)
        i++;
    paths = ft_split(envp[i] + 5, ':');
    i = 0;
    while(paths[i])
    {
        part_path = ft_strjoin(paths[i], "/");
        path = ft_strjoin(part_path, cmd);
        free(part_path);
        if(access(path, F_OK) == 0)
        {
            i++;
            break;
        }
        i++;
    }
    i = -1;
    while(paths[++i])
        free(paths[i]);
    free(paths);
    return path;
}

void execute(char *av,char **envp)
{
    char **cmd;
    int i;
    char *path;

    i = -1;
    cmd = ft_split(av, ' ');
    path = find_path(cmd[0], envp);
    if(!path)
    {
        while(cmd[++i])
            free(cmd[i]);
        free(cmd);
        error();
    }
    if(execve(path, cmd, envp)== -1)
        error();
}

void child_process(char **av,char **envp,int *fd)
{
    int filein;

    filein = open(av[1], O_RDONLY, 0644);
    if(filein == -1)
        error();
    dup2(fd[1], STDOUT_FILENO);
    dup2(filein, STDIN_FILENO);
    close(fd[0]);
    execute(av[2], envp);
}

// int main(int ac, char **av, char **envp)
// {
//     if (ac != 5)
//     {
//         fprintf(stderr, "Usage: %s <infile> <cmd1> <cmd2> <outfile>\\n", av[0]);
//         return EXIT_FAILURE;
//     }

//     int fd[2];
//     pid_t pid1;

//     if(pipe(fd) == -1)
//         error();
//     pid1 = fork();
//     if(pid1 == -1)
//         error();
//     if(pid1 == 0)
//         child_process(av, envp,fd);
//     waitpid(pid1, NULL, 0);
//     parent_process(av, envp, fd);
// }

void execute_pipex(char *input, char **envp) {
    char *cmd1;
    char *cmd2;
    char **split_input;
    char **cmd1_args;
    char **cmd2_args;
    int fd[2];
    pid_t pid1;

    // Split the input at the pipe symbol
    split_input = ft_split(input, '|');
    cmd1 = split_input[0];
    cmd2 = split_input[1];

    cmd1_args = ft_split(cmd1, ' ');
    cmd2_args = ft_split(cmd2, ' ');

    if (pipe(fd) == -1)
        error();
    
    pid1 = fork();
    if (pid1 == -1)
        error();

    if (pid1 == 0)
        child_process(cmd1_args, envp, fd);
    
    waitpid(pid1, NULL, 0);
    parent_process(cmd2_args, envp, fd);

    free(split_input);
    free(cmd1_args);  // Remember to free other dynamically allocated memory as needed
    free(cmd2_args);
}
