/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_str.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nkarpeko <nkarpeko@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/17 14:54:45 by nkarpeko          #+#    #+#             */
/*   Updated: 2023/04/17 16:17:21 by nkarpeko         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft.h"

int	ft_word_count(char *str, char c)
{
	int	word_count;

	word_count = 0;
	while (*str)
	{
		if (*str == c)
		{
			str++;
			continue ;
		}
		word_count++;
		while (*str != c && *str)
			str++;
	}
	return (word_count);
}

void	ft_strings_content(char **str, char *str_copy, char c)
{
	int	i;
	int	word_begin_idx;
	int	str_idx;

	i = 0;
	str_idx = 0;
	while (str_copy[i])
	{
		if (str_copy[i] == c)
		{
			i++;
			continue ;
		}
		word_begin_idx = i;
		while (str_copy[i] != c && str_copy[i])
			i++;
		str[str_idx] = (char *)malloc(i - word_begin_idx + 1);
		ft_strlcpy(str[str_idx], str_copy + word_begin_idx, i - word_begin_idx
			+ 1);
		str_idx++;
	}
}

char	**ft_split(char const *s, char c)
{
	int		words_count;
	char	**str;
	char	*s_copy;

	s_copy = (char *)s;
	words_count = ft_word_count(s_copy, c);
	str = malloc(sizeof(char *) * (words_count + 1));
	if (!str)
		return (0);
	str[words_count] = 0;
	ft_strings_content(str, s_copy, c);
	return (str);
}
