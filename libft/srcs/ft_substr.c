/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_substr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nkarpeko <nkarpeko@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/17 13:12:20 by nkarpeko          #+#    #+#             */
/*   Updated: 2023/04/21 17:16:50 by nkarpeko         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft.h"

char	*ft_substr(char const *s, unsigned int start, size_t len)
{
	size_t			idx;
	char			*substr;
	unsigned int	strlen;

	idx = 0;
	strlen = ft_strlen(s);
	if (!len || !strlen || strlen <= start)
		substr = malloc(1);
	else if (strlen - start < len)
		substr = (char *)malloc(strlen - start + 1);
	else
		substr = (char *)malloc(len + 1);
	if (!substr)
		return (NULL);
	while (start <= strlen && idx < len)
		substr[idx++] = s[start++];
	substr[idx] = '\0';
	return (substr);
}
