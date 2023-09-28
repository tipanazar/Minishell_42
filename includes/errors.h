/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   errors.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nkarpeko <nkarpeko@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/28 21:19:14 by nkarpeko          #+#    #+#             */
/*   Updated: 2023/09/28 21:23:15 by nkarpeko         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ERRORS_H
# define ERRORS_H

# include "../libft/libft.h"
# include "dict.h"
# include <errno.h>
# include <fcntl.h>
# include <stdbool.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>

# define SYSERR "System error\n"

# define C_END "\033[0m"
# define C_BOLD "\033[1m"
# define C_RED "\033[1;31m"

void	msh_destroy(void);

void	error_msg_and_exit(char *function, char *msg);

int		open_ver(const char *file);
ssize_t	read_ver(int fd, void *buf, size_t nbytes);
void	close_ver(int fd);
void	*calloc_ver(size_t count, size_t size);

char	*strdup_ver(const char *str);
char	**ft_split_ver(const char *s, char c);
t_dict	*dict_create_ver(unsigned int len);
void	dict_insert_ver(t_dict *dict, const char *key, void *value);
void	getcwd_ver(char *buf, size_t size);

#endif