/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putstrl.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nkarpeko <nkarpeko@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/22 19:56:39 by nkarpeko          #+#    #+#             */
/*   Updated: 2023/04/23 15:50:04 by nkarpeko         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft.h"

void	ft_putstrl(char *str, int *length)
{
	if (!str)
	{
		ft_putstrl("(null)", length);
		return ;
	}
	while (*str)
		ft_putcharl(*str++, length);
}
