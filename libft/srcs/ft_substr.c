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
	unsigned int	strlength;

	idx = 0;
	strlength = ft_strlen(s);
	if (!len || !strlength || strlength <= start)
		substr = malloc(1);
	else if (strlength - start < len)
		substr = (char *)malloc(strlength - start + 1);
	else
		substr = (char *)malloc(len + 1);
	if (!substr)
		return (NULL);
	while (start <= strlength && idx < len)
		substr[idx++] = s[start++];
	substr[idx] = '\0';
	return (substr);
}
