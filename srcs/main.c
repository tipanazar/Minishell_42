#include "../minishell.h"

void ft_cd(char *buf)
{
  if (buf[0] == 'c' && buf[1] == 'd' && buf[2] == ' ')
    if (chdir(buf + 3) < 0)
      ft_printf("cd: %s: No such file or directory\n", buf + 3);
}

int	main(void)
{
  char *buf;
  int r;
  while (1)
  {
    buf = readline("minishell> ");
    add_history(buf);
    if (!buf || ft_strcmp(buf, "exit") == 0)
      break;
    ft_cd(buf);
    if (fork1() == 0)
    {
      runcmd(parsecmd(buf));
      free(buf);
      exit(0);
    }
    wait(&r);
    free(buf);
  }
  clear_history();
  return 0;
}
