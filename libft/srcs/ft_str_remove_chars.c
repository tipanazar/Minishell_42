/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_str_remove_chars.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: root <root@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/16 22:25:31 by root              #+#    #+#             */
/*   Updated: 2023/12/16 22:28:53 by root             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft.h"

int	count_chars_to_keep(const char *str, const char *chars_to_remove)
{
	int	idx;
	int	count;

	idx = -1;
	count = 0;
	while (str[++idx])
	{
		if (!ft_strchr(chars_to_remove, str[idx]))
			count++;
	}
	return (count);
}

char	*ft_str_remove_chars(char *str, const char *chars_to_remove)
{
	int		idx;
	int		s_idx;
	char	*new_str;
	int		chars_to_keep;

	idx = -1;
	s_idx = 0;
	if (ft_strlen(str) == 0 || ft_strlen(chars_to_remove) == 0)
		return (NULL);
	chars_to_keep = count_chars_to_keep(str, chars_to_remove);
	new_str = (char *)malloc(sizeof(char) * (chars_to_keep + 1));
	if (!new_str)
		return (NULL);
	while (str[++idx])
	{
		if (!ft_strchr(chars_to_remove, str[idx]))
			new_str[s_idx++] = str[idx];
	}
	new_str[s_idx] = '\0';
	return (new_str);
}
