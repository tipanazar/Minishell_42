#include "../libft.h"

char *ft_str_remove_chars(char *str, const char *chars_to_remove)
{
    int idx = -1;
    int s_idx = 0;
    char *new_str;

    while (str[++idx])
        if (!ft_strchr(chars_to_remove, str[idx]))
            s_idx++;
    if (!s_idx)
        return NULL;
    new_str = (char *)malloc(sizeof(char) * (s_idx + 1));
    idx = -1;
    s_idx = 0;

    while (str[++idx])
    {
        if (!ft_strchr(chars_to_remove, str[idx]))
        {
            new_str[s_idx] = str[idx];
            s_idx++;
        }
    }

    new_str[s_idx] = '\0';
    return (new_str);
}
