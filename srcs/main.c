#include "../minishell.h"

void ctrl_c_handler(int sig)
{
  (void)sig;
  rl_on_new_line();
  ft_printf("minishell#\n");
  rl_replace_line("", 0);
  rl_redisplay();
}

int main(void)
{
  char *buf;
  int r;
  signal(SIGINT, ctrl_c_handler);
  signal(SIGQUIT, SIG_IGN);
  while (1)
  {
    buf = readline("minishell# ");
    if (!buf || ft_strcmp(buf, "exit") == 0)
      break;
    if (ft_strlen(buf) && buf[0] != 32)
      add_history(buf);
    builtins(buf);
    if (fork1() == 0)
    {
      runcmd(parsecmd(buf));
      free(buf);
      exit(0);
    }
    wait(&r);
    free(buf);
  }
  rl_clear_history();
  return 0;
}
