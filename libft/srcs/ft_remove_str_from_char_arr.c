/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_remove_str_from_char_arr.c                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: root <root@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/16 22:31:00 by root              #+#    #+#             */
/*   Updated: 2023/12/16 22:32:34 by root             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft.h"

int	count_arr_len_without_target(char **arr, const char *target)
{
	int	idx;
	int	arr_len;

	idx = -1;
	arr_len = 0;
	while (arr[++idx])
	{
		if (ft_strcmp(arr[idx], target))
		{
			arr_len++;
		}
	}
	return (arr_len);
}

void	copy_arr_excluding_target(char **src, char ***dest, const char *target,
		int arr_len)
{
	int	idx;
	int	s_idx;

	idx = -1;
	s_idx = 0;
	*dest = (char **)malloc(sizeof(char *) * (arr_len + 1));
	if (!*dest)
	{
		return ;
	}
	while (src[++idx])
	{
		if (ft_strcmp(src[idx], target))
		{
			(*dest)[s_idx++] = ft_strdup(src[idx]);
		}
	}
	(*dest)[s_idx] = NULL;
}

void	ft_remove_str_from_char_arr(char ***arr, const char *target)
{
	int		arr_len;
	char	**new_arr;

	arr_len = count_arr_len_without_target(*arr, target);
	if (arr_len == ft_strarrlen(*arr))
	{
		return ;
	}
	if (!arr_len)
	{
		ft_free_char_arr(*arr);
		*arr = NULL;
		return ;
	}
	new_arr = NULL;
	copy_arr_excluding_target(*arr, &new_arr, target, arr_len);
	ft_free_char_arr(*arr);
	*arr = new_arr;
}
