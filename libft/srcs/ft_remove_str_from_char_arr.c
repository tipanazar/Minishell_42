/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_remove_str_from_char_arr.c                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: root <root@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/16 22:31:00 by root              #+#    #+#             */
/*   Updated: 2023/12/17 16:48:38 by root             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft.h"

void 	ft_remove_str_from_char_arr(char ***arr, const char *target)
{
    int 	idx = -1;
    int 	s_idx = 0;
    int 	arr_len = 0;
    char 	**new_arr = NULL;

    while ((*arr)[++idx])
        if (ft_strcmp((*arr)[idx], target))
            arr_len++;
    if (arr_len == ft_strarrlen(*arr))
        return;
    if (!arr_len)
    {
        ft_free_char_arr(*arr);
        *arr = NULL;
        return;
    }
    new_arr = (char **)malloc(sizeof(char *) * (arr_len + 1));
    if (!new_arr)
        return;
    idx = -1;
    while ((*arr)[++idx])
    {
        if (ft_strcmp((*arr)[idx], target))
            new_arr[s_idx++] = ft_strdup((*arr)[idx]);
    }
    new_arr[s_idx] = NULL;
    ft_free_char_arr(*arr);
    *arr = new_arr;
}
