#include "../libft.h"

void ft_print_char_arr(char **str)
{
    int i = 0;
    while (str[i])
    {
        ft_printf("%s\n", str[i]);
        i++;
    }
    ft_printf("\n");
}