/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putaddress.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nkarpeko <nkarpeko@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/23 15:11:54 by nkarpeko          #+#    #+#             */
/*   Updated: 2023/04/23 16:06:51 by nkarpeko         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft.h"

void	ft_putaddress(unsigned long num, int *length)
{
	if (!num)
		ft_putstrl("(nil)", length);
	else
	{
		if (num > 15)
			ft_putaddress(num / 16, length);
		else
			ft_putstrl("0x", length);
		ft_putcharl("0123456789abcdef"[num % 16], length);
	}
}
