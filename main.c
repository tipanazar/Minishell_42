/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: viktortr <viktortr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/20 18:40:19 by viktortr          #+#    #+#             */
/*   Updated: 2023/10/03 18:47:26 by viktortr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void procces_command(char *input)
{
    pid_t pid;
    int status = 0;
    
    if (!input || ft_strlen(input) == 0)
    {
        free(input);
        return;
    }

    pid = fork();

    add_history(input);
    
    if (pid == 0)
    {
        char *args[] = {input, NULL};
        execvp(args[0], args);
        fprintf(stderr, "minishell> %s: command not found\n", input);
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

int	main(void)
{
	char *input;

	while (1)
	{
		input = readline("minishell> ");
		if (!input)
		{
			free(input);
			break ;
		}
		else if(ft_strcmp(input, "exit") == 0)
		{
			free(input);
			break;
		}
        set_flags(input);
		procces_command(input);
		
	}
	return (0);
}