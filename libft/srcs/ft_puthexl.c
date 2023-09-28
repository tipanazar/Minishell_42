/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_puthexl.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nkarpeko <nkarpeko@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/23 13:45:42 by nkarpeko          #+#    #+#             */
/*   Updated: 2023/04/23 15:21:47 by nkarpeko         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft.h"

void	ft_puthexl(unsigned int num, int *length, int isbig)
{
	if (num > 15)
		ft_puthexl(num / 16, length, isbig);
	if (isbig)
		ft_putcharl("0123456789ABCDEF"[num % 16], length);
	else
		ft_putcharl("0123456789abcdef"[num % 16], length);
}
