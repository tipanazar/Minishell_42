#include "../libft.h"

void ft_print_str_arr(char **str)
{
    int i = 0;
    if(!str)
        return;
    while (str[i])
    {
        ft_printf("%s\n", str[i]);
        i++;
    }
    ft_printf("\n");
}