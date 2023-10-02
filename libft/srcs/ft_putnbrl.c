/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putnbrl.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nkarpeko <nkarpeko@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/22 20:33:52 by nkarpeko          #+#    #+#             */
/*   Updated: 2023/04/23 15:32:15 by nkarpeko         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft.h"

void	ft_putnbrl(long nb, int *length)
{
	if (nb == -2147483648)
	{
		ft_putnbrl(nb / 10, length);
		ft_putcharl('8', length);
	}
	else if (nb < 0)
	{
		ft_putcharl('-', length);
		ft_putnbrl(-nb, length);
	}
	else
	{
		if (nb > 9)
			ft_putnbrl(nb / 10, length);
		ft_putcharl(nb % 10 + '0', length);
	}
}
