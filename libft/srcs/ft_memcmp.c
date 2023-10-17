/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcmp.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nkarpeko <nkarpeko@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/14 15:20:39 by nkarpeko          #+#    #+#             */
/*   Updated: 2023/04/21 17:16:50 by nkarpeko         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft.h"

int	ft_memcmp(void *s1, void *s2, unsigned int n)
{
	unsigned char	*p1;
	unsigned char	*p2;
	unsigned int	idx;

	p1 = s1;
	p2 = s2;
	idx = 0;
	while (idx < n)
	{
		if (*p1 != *p2)
		{
			if (*p1 < *p2)
			{
				return (-1);
			}
			else
			{
				return (1);
			}
		}
		idx++;
		p1++;
		p2++;
	}
	return (0);
}
