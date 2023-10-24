#include "../minishell.h"

void ft_cd(char *buf)
{
    int idx = 2;
    int flag = 1;
    char *home_dir;
    ft_printf("buf: %s\n", buf);
    while (buf[idx])
    {
        if (!ft_isspace(buf[idx])) //! ????
            flag = 0;
        idx++;
    }
    if (flag)
    {
        home_dir = getenv("HOME");
        if (home_dir)
            chdir(home_dir);
    }
    else
    {
        ft_trim_leading_spaces(buf + 2);
        if (chdir(buf + 2) < 0)
            ft_printf("cd: %s: No such file or directory\n", buf + 2);
    }
}

int builtins(char *buf)
{
    ft_trim_leading_spaces(buf);
    if (ft_strncmp(buf, "cd", 2) == 0)
    {
        ft_cd(buf);
        return 1;
    }
    // if (ft_strncmp(buf, "echo ", 5) == 0)
    //     ft_printf("%s\n", buf + 5);
    return 0;
}
