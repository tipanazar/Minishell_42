/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: viktortr <viktortr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/20 18:40:19 by viktortr          #+#    #+#             */
/*   Updated: 2023/09/21 18:28:42 by viktortr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	main(void)
{
	char *command;

	while (1)
	{
		command = readline("minishell$ ");
		if (!command || ft_strlen(command) == 0)
		{
			free(command);
			continue ;
		}
		add_history(command);
		if (ft_strcmp(command, "exit") == 0)
		{
			free(command);
			break ;
		}
		else
			free(command);
	}

	return (0);
}