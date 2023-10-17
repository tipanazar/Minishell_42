#include "../../minishell.h"

int fork1(void)
{
    int pid;

    pid = fork();
    if (pid == -1)
        perror("fork");
    return pid;
}

int peek(char **ps, char *es, char *toks)
{
    char *s;
    char whitespace[6] = " \t\r\n\v";

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
