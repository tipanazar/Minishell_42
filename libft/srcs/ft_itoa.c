/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nkarpeko <nkarpeko@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/17 16:55:47 by nkarpeko          #+#    #+#             */
/*   Updated: 2023/04/21 17:16:50 by nkarpeko         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft.h"

int	get_total_lenght(int n)
{
	int	total;

	total = 1;
	if (n <= -1 && n >= -9)
		return (2);
	if (n == -2147483648)
	{
		n /= 10;
		total++;
	}
	while (n > 9 || n < -9)
	{
		if (n > 0)
		{
			if (n > 9)
				n /= 10;
			total++;
		}
		else
		{
			total++;
			n = -n;
		}
	}
	return (total);
}

void	middleware(int *num, char *str, int *total_length)
{
	str[*total_length] = '\0';
	if (*num == -2147483648)
	{
		*num = *num / 10;
		str[--*total_length] = '8';
	}
}

char	*ft_itoa(int n)
{
	int		total_length;
	char	*str;
	int		step;

	step = 0;
	total_length = get_total_lenght(n);
	str = (char *)malloc(total_length + 1);
	if (!str)
		return (NULL);
	middleware(&n, str, &total_length);
	while (total_length--)
	{
		if (n >= 0)
		{
			str[total_length + step] = n % 10 + 48;
			n /= 10;
		}
		else
		{
			str[0] = '-';
			n = -n;
			step = 1;
		}
	}
	return (str);
}
