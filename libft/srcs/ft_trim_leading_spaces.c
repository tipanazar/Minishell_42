#include "../libft.h"

void	ft_trim_leading_spaces(char *str)
{
	int	f_idx;
	int	s_idx;

	f_idx = 0;
	s_idx = 0;
	if (!str)
		return ;
	while (str[f_idx] && ft_isspace(str[f_idx]))
		f_idx++;
	while (str[f_idx])
		str[s_idx++] = str[f_idx++];
	while (str[s_idx])
		str[s_idx++] = '\0';
}
