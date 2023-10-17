/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strnstr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nkarpeko <nkarpeko@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/14 15:21:24 by nkarpeko          #+#    #+#             */
/*   Updated: 2023/04/21 17:16:50 by nkarpeko         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft.h"

char	*ft_strnstr(const char *big, const char *little, size_t len)
{
	size_t	i;
	size_t	j;
	char	*big_c;
	char	*lil_c;

	big_c = (char *)big;
	lil_c = (char *)little;
	i = -1;
	if (!*little)
		return (big_c);
	while (big_c[++i] && i < len)
	{
		j = 0;
		while (big_c[i + j] == lil_c[j] && i + j < len)
		{
			if (lil_c[j + 1] == '\0')
				return (big_c + i);
			j++;
		}
	}
	return (0);
}
