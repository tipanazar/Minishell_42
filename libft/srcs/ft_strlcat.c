/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcat.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nkarpeko <nkarpeko@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/14 15:21:13 by nkarpeko          #+#    #+#             */
/*   Updated: 2023/04/17 20:48:33 by nkarpeko         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

unsigned int	ft_strlcat(char *dest, const char *src, unsigned int size)
{
	unsigned int	d_idx;
	unsigned int	s_idx;
	unsigned int	res;

	d_idx = 0;
	s_idx = 0;
	res = 0;
	while (dest[d_idx] != '\0')
		d_idx++;
	while (src[res] != '\0')
		res++;
	if (size <= d_idx)
		res += size;
	else
		res += d_idx;
	while (src[s_idx] != '\0' && (d_idx + 1) < size)
	{
		dest[d_idx] = src[s_idx];
		d_idx++;
		s_idx++;
	}
	dest[d_idx] = '\0';
	return (res);
}
