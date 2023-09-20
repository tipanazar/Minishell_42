/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nkarpeko <nkarpeko@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/23 19:05:07 by nkarpeko          #+#    #+#             */
/*   Updated: 2023/05/05 15:13:03 by nkarpeko         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft.h"

char	*get_next_line(int fd)
{
	static char	buffer[BUFFER_SIZE + 1];
	char		*return_str;
	int			idx;

	if (fd < 0 || !BUFFER_SIZE)
		return (NULL);
	if (read(fd, 0, 0) < 0)
	{
		idx = 0;
		while (buffer[idx])
			buffer[idx++] = '\0';
		return (NULL);
	}
	return_str = NULL;
	while (buffer[0] || read(fd, buffer, BUFFER_SIZE))
	{
		return_str = ft_strjoin_to_new_line(return_str, buffer);
		ft_remove_first_line(buffer);
		if (ft_new_line_detector(buffer) || ft_new_line_detector(return_str))
			break ;
	}
	return (return_str);
}
