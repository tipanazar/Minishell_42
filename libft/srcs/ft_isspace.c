#include "../libft.h"

int	ft_isspace(char ch)
{
	return (ch == ' ' || (ch >= 9 && ch <= 13));
}
