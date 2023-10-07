#include "../minishell.h"

#pragma GCC diagnostic ignored "-Winfinite-recursion"

void runcmd(struct cmd *cmd)
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
  exit(0);
}

int getcmd(char *buf, int nbuf)
{

  if (isatty(fileno(stdin)))
    write(2, "minishell> ", 11);
  ft_memset(buf, 0, nbuf);
  fgets(buf, nbuf, stdin);
  if (buf[0] == 0) // EOF
    return -1;
  return 0;
}

int fork1(void)
{
  int pid;

  pid = fork();
  if (pid == -1)
    perror("fork");
  return pid;
}

struct cmd *execcmd(void)
{
  struct execcmd *cmd;

  cmd = malloc(sizeof(*cmd));
  ft_memset(cmd, 0, sizeof(*cmd));
  cmd->type = ' ';
  return (struct cmd *)cmd;
}

struct cmd *redircmd(struct cmd *subcmd, char *file, int type)
{
  struct redircmd *cmd;

  cmd = malloc(sizeof(*cmd));
  ft_memset(cmd, 0, sizeof(*cmd));
  cmd->type = type;
  cmd->cmd = subcmd;
  cmd->file = file;
  cmd->mode = (type == '<') ? O_RDONLY : O_WRONLY | O_CREAT | O_TRUNC;
  cmd->fd = (type == '<') ? 0 : 1;
  return (struct cmd *)cmd;
}

struct cmd *pipecmd(struct cmd *left, struct cmd *right)
{
  struct pipecmd *cmd;

  cmd = malloc(sizeof(*cmd));
  ft_memset(cmd, 0, sizeof(*cmd));
  cmd->type = '|';
  cmd->left = left;
  cmd->right = right;
  return (struct cmd *)cmd;
}

// Parsing

int gettoken(char **ps, char *es, char **q, char **eq)
{
  char *s;
  int ret;
  char whitespace[] = " \t\r\n\v";
  char symbols[] = "<|>";

  s = *ps;
  while (s < es && ft_strchr(whitespace, *s))
    s++;
  if (q)
    *q = s;
  ret = *s;
  switch (*s)
  {
  case 0:
    break;
  case '|':
  case '<':
    s++;
    break;
  case '>':
    s++;
    break;
  default:
    ret = 'a';
    while (s < es && !ft_strchr(whitespace, *s) && !ft_strchr(symbols, *s))
      s++;
    break;
  }
  if (eq)
    *eq = s;

  while (s < es && ft_strchr(whitespace, *s))
    s++;
  *ps = s;
  return ret;
}

int peek(char **ps, char *es, char *toks)
{
  char *s;
  char whitespace[] = " \t\r\n\v";

  s = *ps;
  while (s < es && ft_strchr(whitespace, *s))
    s++;
  *ps = s;
  return *s && ft_strchr(toks, *s);
}

// make a copy of the characters in the input buffer, starting from s through es.
// null-terminate the copy to make it a string.
char *mkcopy(char *s, char *es)
{
  int n = es - s;
  char *c = malloc(n + 1);
  if (c == NULL)
  {
    write(2, "Memory allocation failed.\n", 25);
    exit(1);
  }
  strncpy(c, s, n);
  c[n] = 0;
  return c;
}

struct cmd *parsecmd(char *s)
{
  char *es;
  struct cmd *cmd;

  es = s + ft_strlen(s);
  cmd = parseline(&s, es);
  peek(&s, es, "");
  if (s != es)
  {
    write(2, "leftovers: %s\n", 14);
    exit(-1);
  }
  return cmd;
}

struct cmd *parseline(char **ps, char *es)
{
  struct cmd *cmd;
  cmd = parsepipe(ps, es);
  return cmd;
}

struct cmd *parsepipe(char **ps, char *es)
{
  struct cmd *cmd;

  cmd = parseexec(ps, es);
  if (peek(ps, es, "|"))
  {
    gettoken(ps, es, 0, 0);
    cmd = pipecmd(cmd, parsepipe(ps, es));
  }
  return cmd;
}

struct cmd *parseredirs(struct cmd *cmd, char **ps, char *es)
{
  int tok;
  char *q, *eq;

  while (peek(ps, es, "<>"))
  {
    tok = gettoken(ps, es, 0, 0);
    if (gettoken(ps, es, &q, &eq) != 'a')
    {
      write(2, "missing file for redirection\n", 28);
      exit(-1);
    }
    switch (tok)
    {
    case '<':
      cmd = redircmd(cmd, mkcopy(q, eq), '<');
      break;
    case '>':
      cmd = redircmd(cmd, mkcopy(q, eq), '>');
      break;
    }
  }
  return cmd;
}

struct cmd *parseexec(char **ps, char *es)
{
  char *q, *eq;
  int tok, argc;
  struct execcmd *cmd;
  struct cmd *ret;

  ret = execcmd();
  cmd = (struct execcmd *)ret;

  argc = 0;
  ret = parseredirs(ret, ps, es);
  while (!peek(ps, es, "|"))
  {
    if ((tok = gettoken(ps, es, &q, &eq)) == 0)
      break;
    if (tok != 'a')
    {
      write(2, "syntax error\n", 12);
      exit(-1);
    }
    cmd->argv[argc] = mkcopy(q, eq);
    argc++;
    if (argc >= MAXARGS)
    {
      write(2, "too many args\n", 14);
      exit(-1);
    }
    ret = parseredirs(ret, ps, es);
  }
  cmd->argv[argc] = 0;
  return ret;
}

// int main(void)
// {
//   static char buf[100];
//   int r;

//   while (getcmd(buf, sizeof(buf)) >= 0)
//   {
//     if (buf[0] == 'c' && buf[1] == 'd' && buf[2] == ' ')
//     {
//       buf[ft_strlen(buf) - 1] = 0;
//       if (chdir(buf + 3) < 0)
//         write(2, "cannot cd %s\n", 13);
//       continue;
//     }
//     if (fork1() == 0)
//       runcmd(parsecmd(buf));
//     wait(&r);
//   }
//   exit(0);
// }

int main(void)
{
    char *buf;
    int r;

    while (1) // Infinite loop, can be broken by certain conditions within
    {
        buf = readline("minishell> "); // Prompt user and read input

        if (!buf) // If EOF (Ctrl+D) is detected, exit
            break;
        
        if (buf[0] == 'c' && buf[1] == 'd' && buf[2] == ' ')
        {
            if (chdir(buf + 3) < 0)
                write(2, "cannot cd %s\n", 13);
            free(buf); // Deallocate memory allocated by readline
            continue;
        }

        if (fork1() == 0)
        {
            runcmd(parsecmd(buf));
            free(buf); // Deallocate memory allocated by readline in child process
            exit(0); // Ensure child process exits
        }
        wait(&r);

        free(buf); // Deallocate memory allocated by readline
    }
    return 0;
}