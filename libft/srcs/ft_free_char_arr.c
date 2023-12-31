/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_free_char_arr.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: root <root@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/13 17:59:19 by nkarpeko          #+#    #+#             */
/*   Updated: 2023/12/16 22:24:56 by root             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft.h"

void	ft_free_char_arr(char **arr)
{
	int	idx;

	idx = -1;
	if (!arr)
		return ;
	if (*arr)
	{
		while (arr[++idx])
			free(arr[idx]);
		free(arr);
	}
	arr = NULL;
}
