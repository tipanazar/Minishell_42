#include "../../minishell.h"

int gettoken(char **ps, char *es, char **q, char **eq)
{
  char *s;
  int ret;
  char whitespace[6] = " \t\r\n\v";
  char symbols[4] = "<|>";

  s = *ps;
  while (s < es && ft_strchr(whitespace, *s))
    s++;
  if (q)
    *q = s;
  ret = *s;

  if (*s == 0)
  {
  }
  else if (*s == '|' || *s == '<')
    s++;
  else if (*s == '>')
    s++;
  else
  {
    ret = 'a';
    while (s < es && !ft_strchr(whitespace, *s) && !ft_strchr(symbols, *s))
      s++;
  }

  if (eq)
    *eq = s;

  while (s < es && ft_strchr(whitespace, *s))
    s++;
  *ps = s;
  return ret;
}