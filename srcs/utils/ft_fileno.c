#include "../../minishell.h"
#include <stdio.h>

#define MAX_FILES 256

typedef struct s_FileMapping
{
	FILE		*stream;
	int			fd;
}				t_FileMapping;

t_FileMapping	*get_file_map(void)
{
	static t_FileMapping	file_map[MAX_FILES] = {{0}};

	return (file_map);
}

FILE	*ft_fopen(const char *filename, const char *mode)
{
	FILE			*f;
	int				i;
	t_FileMapping	*file_map;

	file_map = get_file_map();
	f = fopen(filename, mode);
	if (f)
	{
		i = 0;
		while (i < MAX_FILES && file_map[i].stream)
			i++;
		if (i < MAX_FILES)
		{
			file_map[i].stream = f;
			file_map[i].fd = fileno(f);
		}
	}
	return (f);
}

int	ft_fclose(FILE *stream)
{
	int				i;
	t_FileMapping	*file_map;

	file_map = get_file_map();
	i = 0;
	while (i < MAX_FILES && file_map[i].stream != stream)
		i++;
	if (i < MAX_FILES)
		file_map[i].stream = NULL;
	return (fclose(stream));
}

int	ft_fileno(FILE *stream)
{
	int				i;
	t_FileMapping	*file_map;

	file_map = get_file_map();
	i = 0;
	while (i < MAX_FILES && file_map[i].stream != stream)
		i++;
	if (i < MAX_FILES)
		return (file_map[i].fd);
	return (-1);
}
