/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: viktortr <viktortr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/20 18:40:49 by viktortr          #+#    #+#             */
/*   Updated: 2023/09/30 21:27:15 by viktortr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "./libft/libft.h"
# include <dirent.h>
# include <fcntl.h>
# include <readline/history.h>
# include <readline/readline.h>
# include <signal.h>
# include <stdio.h>
# include <stdlib.h>
# include <sys/ioctl.h>
# include <sys/stat.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <termcap.h>
# include <termios.h>
# include <unistd.h>

typedef struct s_shell_flags
{
	int is_input_redirected;  // (<)
	int is_output_redirected; // (>)
	int is_output_append;     // (>>)
	int is_heredoc;           // (<<)
	int is_single_quoted;     // (')
	int is_double_quoted;     // (")
	int is_piped;             // (|)
}		t_shell_flags;

//? Pipex
void    execute_pipex(char *input, char **envp);
void    error(void);
void    child_process(char **av, char **envp, int *fd);
void    parent_process(char **av, char **envp, int *fd);
//? pipexc


void	set_flags(char *str);

#endif