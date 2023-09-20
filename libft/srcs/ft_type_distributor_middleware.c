/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_type_distributor_middleware.c                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nkarpeko <nkarpeko@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/20 19:22:30 by nkarpeko          #+#    #+#             */
/*   Updated: 2023/04/23 15:44:43 by nkarpeko         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft.h"

void	ft_type_distributor_middleware(va_list args, char ch, int *length)
{
	if (ch == 'c')
		ft_putcharl(va_arg(args, int), length);
	else if (ch == 's')
		ft_putstrl(va_arg(args, char *), length);
	else if (ch == 'p')
		ft_putaddress(va_arg(args, unsigned long), length);
	else if (ch == 'd')
		ft_putnbrl(va_arg(args, int), length);
	else if (ch == 'i')
		ft_putnbrl(va_arg(args, int), length);
	else if (ch == 'u')
		ft_putnbrl(va_arg(args, unsigned int), length);
	else if (ch == 'x')
		ft_puthexl(va_arg(args, int), length, 0);
	else if (ch == 'X')
		ft_puthexl(va_arg(args, int), length, 1);
	else if (ch == '%')
		ft_putcharl('%', length);
}
