/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_trim_leading_spaces.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: root <root@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/14 15:21:36 by nkarpeko          #+#    #+#             */
/*   Updated: 2023/10/31 15:18:55 by root             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft.h"

char	*ft_trim_leading_spaces(const char *str)
{
	int		f_idx;
	char	*trimmed_str;

	f_idx = 0;
	if (!str)
		return (NULL);
	while (str[f_idx] && ft_isspace(str[f_idx]))
		f_idx++;
	trimmed_str = ft_strdup(str + f_idx);
	return (trimmed_str);
}
