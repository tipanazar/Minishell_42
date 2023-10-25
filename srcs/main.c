#include "../minishell.h"

void ctrl_c_handler(int sig)
{
  (void)sig;
  rl_on_new_line();
  ft_printf("minishell#\n");
  rl_replace_line("", 0);
  rl_on_new_line();
  rl_redisplay();
}

bool is_blank(const char *buf)
{
  if (!buf)
    return (true);
  while (*buf)
  {
    if (!ft_isspace(*buf))
      return (false);
    buf++;
  }
  return (true);
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
    if (!buf)
      break;
    if (ft_strcmp(buf, "exit") == 0)
    {
      free(buf);
      break;
    }
    if (ft_strlen(buf) && ft_isspace(buf[0]) == 0)
      add_history(buf);
    if (builtins(buf))
      continue;
    if (!buf || is_blank(buf))
    {
      free(buf);
      continue;
    }
    if (fork1() == 0)
    {
      runcmd(parsecmd(buf));
      free(buf);
      buf = NULL;
      exit(0);
    }
    wait(&r);
    free(buf);
  }
  rl_clear_history();
  return (0);
}
