/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nkarpeko <nkarpeko@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/17 14:01:03 by nkarpeko          #+#    #+#             */
/*   Updated: 2023/04/21 17:16:50 by nkarpeko         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft.h"

char	*ft_strjoin(char const *s1, char const *s2)
{
	int		idx;
	char	*result_str;

	idx = 0;
	result_str = (char *)malloc(ft_strlen(s1) + ft_strlen(s2) + 1);
	if (!result_str)
		return (NULL);
	while (*s1)
		result_str[idx++] = *s1++;
	while (*s2)
		result_str[idx++] = *s2++;
	result_str[idx] = '\0';
	return (result_str);
}
