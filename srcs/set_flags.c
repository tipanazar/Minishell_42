#include "../minishell.h"

void	set_flags(char *str)
{
	t_shell_flags	*flag;
	char			**input;
	int				i;

	i = -1;
	// Allocate memory for the flag structure
	flag = (t_shell_flags *)malloc(sizeof(t_shell_flags));
	if (!flag)
		return ;
	// Initialize structure members
	flag->is_piped = 0;
	flag->is_input_redirected = 0;
	flag->is_output_redirected = 0;
	flag->is_output_append = 0;
	flag->is_heredoc = 0;
	input = ft_split(str, ' ');
	while (input[++i])
	{
		if (ft_strcmp(input[i], "|") == 0)
			flag->is_piped += 1;
		if (ft_strcmp(input[i], "<") == 0)
			flag->is_input_redirected += 1;
		if (ft_strcmp(input[i], ">") == 0)
			flag->is_output_redirected += 1;
		if (ft_strcmp(input[i], ">>") == 0)
			flag->is_output_append += 1;
		if (ft_strcmp(input[i], "<<") == 0)
			flag->is_heredoc += 1;
	}
}

void	free_flags(t_shell_flags *flag, char **input)
{
	int i = -1;

	free(flag);
	while (input[++i])
		free(input[i]);
	free(input);
}