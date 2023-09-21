/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nkarpeko <nkarpeko@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/17 14:30:35 by nkarpeko          #+#    #+#             */
/*   Updated: 2023/04/21 17:16:50 by nkarpeko         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft.h"

char	*ft_strtrim(char const *s1, char const *set)
{
	int		idx;
	char	*result;
	int		start;
	int		end;

	idx = 0;
	if (!s1)
		return (NULL);
	while (s1[idx] && ft_strchr(set, s1[idx]))
		idx++;
	start = idx;
	idx = ft_strlen(s1);
	while (idx > start && ft_strchr(set, s1[idx - 1]))
		idx--;
	end = idx;
	result = ft_substr(s1, start, end - start);
	return (result);
}
