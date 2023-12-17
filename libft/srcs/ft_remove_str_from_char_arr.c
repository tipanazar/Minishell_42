/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_remove_str_from_char_arr.c                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: root <root@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/16 22:31:00 by root              #+#    #+#             */
/*   Updated: 2023/12/17 16:48:38 by root             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft.h"

typedef struct s_cmd
{
	char		**src;
	char		***dest;
	const char	*target;
	size_t		target_len;
}				t_CopyParams;

int	calculate_arr_len_without_target(char **arr, const char *target,
		size_t target_len)
{
	int	arr_len;
	int	i;

	arr_len = 0;
	i = 0;
	while (arr[i])
	{
		if (ft_strncmp(arr[i], target, target_len) != 0
			|| arr[i][target_len] != '=')
			arr_len++;
		i++;
	}
	return (arr_len);
}

void	copy_arr_excluding_target(t_CopyParams *params, int arr_len)
{
	int	i;
	int	j;

	i = 0;
	j = 0;
	*(params->dest) = (char **)malloc(sizeof(char *) * (arr_len + 1));
	if (!*(params->dest))
		return ;
	while (params->src[i])
	{
		if (ft_strncmp(params->src[i], params->target, params->target_len) != 0
			|| params->src[i][params->target_len] != '=')
		{
			(*(params->dest))[j++] = ft_strdup(params->src[i]);
		}
		i++;
	}
	(*(params->dest))[j] = NULL;
}

void	ft_remove_str_from_char_arr(char ***arr, const char *target)
{
	char			**new_arr;
	size_t			target_len;
	int				arr_len;
	t_CopyParams	params;

	if (!arr || !*arr || !target)
		return ;
	target_len = ft_strlen(target);
	arr_len = calculate_arr_len_without_target(*arr, target, target_len);
	params.src = *arr;
	params.dest = &new_arr;
	params.target = target;
	params.target_len = target_len;
	copy_arr_excluding_target(&params, arr_len);
	ft_free_char_arr(*arr);
	*arr = new_arr;
}
