#include "../libft.h"

void ft_str_remove_chars(char *str, const char *chars_to_remove)
{
    char *src = str;
    char *dest = str;

    while (*src)
    {
        int should_remove = 0;
        const char *ptr = chars_to_remove;

        while (*ptr)
        {
            if (*src == *ptr)
            {
                should_remove = 1;
                break;
            }
            ++ptr;
        }

        if (!should_remove)
        {
            *dest++ = *src;
        }

        ++src;
    }

    *dest = '\0'; // Null-terminate the modified string
}
