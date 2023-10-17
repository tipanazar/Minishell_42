#include "../../minishell.h"

int runcmd(struct cmd *cmd)
{
    int fd_redirect;
    int p_id;
    int fd_pipe[2];
    struct execcmd *ecmd;
    struct pipecmd *pcmd;
    struct redircmd *rcmd;

    if (cmd == 0)
        exit(0);

    char type = cmd->type;

    if (type == ' ')
    {
        ecmd = (struct execcmd *)cmd;
        if (ecmd->argv[0] == 0)
            exit(0);
        execvp(ecmd->argv[0], ecmd->argv);
    }
    else if (type == '>' || type == '<')
    {
        rcmd = (struct redircmd *)cmd;
        if (rcmd->type == '>')
        {
            if ((fd_redirect = open(rcmd->file, rcmd->mode, 0666)) < 0)
            {
                write(STDERR_FILENO, "open ", 5);
                write(STDERR_FILENO, rcmd->file, ft_strlen(rcmd->file));
                write(STDERR_FILENO, " has failed\n", 11);
                exit(0);
            }
        }
        else if (rcmd->type == '<')
        {
            if ((fd_redirect = open(rcmd->file, rcmd->mode)) < 0)
            {
                write(STDERR_FILENO, "open ", 5);
                write(STDERR_FILENO, rcmd->file, ft_strlen(rcmd->file));
                write(STDERR_FILENO, " has failed\n", 11);
                exit(0);
            }
        }
        if (dup2(fd_redirect, rcmd->fd) < 0)
        {
            write(2, "dup2 has failed\n", 15);
            exit(0);
        }
        runcmd(rcmd->cmd);
    }
    else if (type == '|')
    {
        pcmd = (struct pipecmd *)cmd;
        if (pipe(fd_pipe) < 0)
        {
            write(2, "pipe has failed\n", 14);
            exit(0);
        }
        if ((p_id = fork()) < 0)
        {
            write(2, "fork has failed\n", 14);
            exit(0);
        }
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
    }
    else
    {
        write(2, "unknown runcmd\n", 15);
        exit(1);
    }
    // exit(0);
    return 1;
}