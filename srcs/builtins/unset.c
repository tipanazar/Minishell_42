#include "../../minishell.h"

void unset(char **buf_arr, char ***custom_environ)
{
    int idx;
    idx = -1;
    char *to_delete;
    while (buf_arr[++idx])
    {
        to_delete = custom_getenv(buf_arr[idx], *custom_environ, true);
        if(to_delete)
            ft_remove_str_from_char_arr(custom_environ, to_delete);
        to_delete = NULL;
    }
    g_exit_code = 0;
}
