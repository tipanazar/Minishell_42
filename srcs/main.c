/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nkarpeko <nkarpeko@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/20 18:40:19 by viktortr          #+#    #+#             */
/*   Updated: 2023/10/02 17:55:10 by nkarpeko         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

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