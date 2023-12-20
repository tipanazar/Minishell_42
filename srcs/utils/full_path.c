#include "../../minishell.h"

char	*create_full_path(char *dir, char *command)
{
	char	*temp_path;
	char	*full_path;

	temp_path = ft_strjoin(dir, "/");
	if (!temp_path)
		return (NULL);
	full_path = ft_strjoin(temp_path, command);
	free(temp_path);
	return (full_path);
}

char	*search_command_in_dirs(char *temp, char *command)
{
	char		*dir;
	struct stat	st;
	char		*full_path;

	dir = ft_strtok(temp, ":");
	while (dir)
	{
		full_path = create_full_path(dir, command);
		if (!full_path)
			return (NULL);
		if (stat(full_path, &st) == 0 && (st.st_mode & S_IXUSR))
			return (full_path);
		free(full_path);
		dir = ft_strtok(NULL, ":");
	}
	return (NULL);
}

char	*find_command_in_path(char *command)
{
	struct stat	st;
	char		*path;
	char		*temp;
	char		*result;

	if (command[0] == '/' || ft_strchr(command, '/'))
	{
		if (stat(command, &st) == 0 && (st.st_mode & S_IXUSR))
			return (ft_strdup(command));
		return (NULL);
	}
	path = getenv("PATH");
	if (!path)
		return (NULL);
	temp = ft_strdup(path);
	if (!temp)
		return (NULL);
	result = search_command_in_dirs(temp, command);
	free(temp);
	return (result);
}

void	pwd(void)
{
	char	dir[2000];

	if (getcwd(dir, sizeof(dir)) != NULL)
	{
		ft_printf("%s\n", dir);
		g_exit_code = 0;
	}
	else
	{
		perror("pwd");
		g_exit_code = 1;
	}
}
