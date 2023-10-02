/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nkarpeko <nkarpeko@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/20 18:40:19 by viktortr          #+#    #+#             */
/*   Updated: 2023/10/02 17:52:14 by nkarpeko         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	main(void)
{
	char *input;
	pid_t pid;
	int status = 0;

	while (1)
	{
		input = readline("minishell> ");
		if (!input)
		{
			free(input);
			break ;
		}

		add_history(input);
		pid = fork();

		if (pid == 0)
		{
			char *args[] = {"/bin/sh", "-c", input, NULL};
			execve(args[0], args, NULL);
			perror("execve");
			exit(1);
		}
		else if (pid < 0)
			perror("fork");
		else
		{
			if (waitpid(pid, &status, WUNTRACED) == -1)
				perror("waitpid");
		}
		free(input);
	}
	return (0);
}