#include "../libft.h"

// void ft_trim_leading_spaces(char *str)
// {
//     int f_idx = 0;
//     int s_idx = 0;
//     int len;
//     if (!str)
//         return;
//     while (str[f_idx] && ft_isspace(str[f_idx]))
//         f_idx++;
//     if (f_idx == 0)
//         return;
//     len = ft_strlen(str);
//     while (s_idx <= len - f_idx)
//         str[s_idx++] = str[f_idx++];
//     str[s_idx] = '\0';    
// }

void ft_trim_leading_spaces(char *str)
{
    int f_idx = 0;
    int s_idx = 0;
    if (!str)
        return;
    while (str[f_idx] && ft_isspace(str[f_idx]))
        f_idx++;
    while (str[f_idx])
        str[s_idx++] = str[f_idx++];
    while (str[s_idx])
        str[s_idx++] = '\0';
}
