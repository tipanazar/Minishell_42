/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memmove.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nkarpeko <nkarpeko@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/14 15:20:46 by nkarpeko          #+#    #+#             */
/*   Updated: 2023/04/21 17:16:50 by nkarpeko         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft.h"

void	*ft_memmove(void *dest, void *src, int num)
{
	unsigned char	*p_dest;
	unsigned char	*p_src;

	p_dest = dest;
	p_src = src;
	if (!dest && !src)
		return (NULL);
	if (p_src < p_dest)
	{
		p_dest += num - 1;
		p_src += num - 1;
		while (num--)
		{
			*p_dest-- = *p_src--;
		}
	}
	else
	{
		while (num--)
		{
			*p_dest++ = *p_src++;
		}
	}
	return (dest);
}
