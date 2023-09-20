/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   libft.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nkarpeko <nkarpeko@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/02 09:50:54 by nkarpeko          #+#    #+#             */
/*   Updated: 2023/06/27 18:03:34 by nkarpeko         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LIBFT_H
# define LIBFT_H

# include <stdarg.h>
# include <stdbool.h>
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>

# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 9999
# endif

typedef struct s_list
{
	void			*content;
	struct s_list	*next;
}					t_list;

void				ft_bzero(void *ptr, int num);
void				*ft_calloc(int num, int size);
int					ft_isalnum(int symbol);
int					ft_isalpha(int ch);
int					ft_isascii(int num);
int					ft_isdigit(int num);
int					ft_isprint(int symbol);
void				*ft_memchr(void *s, int c, int n);
int					ft_memcmp(void *s1, void *s2, unsigned int n);
void				*ft_memcpy(void *dest, const void *src, size_t num);
void				*ft_memmove(void *dest, void *src, int num);
void				*ft_memset(void *ptr, int value, int num);
char				*ft_strchr(const char *s, char c);
char				*ft_strdup(const char *str);
unsigned int		ft_strlcat(char *dest, const char *src, unsigned int size);
unsigned int		ft_strlcpy(char *dest, const char *src, unsigned int size);
size_t				ft_strlen(const char *str);
int					ft_strncmp(const char *s1, const char *s2, size_t n);
char				*ft_strnstr(const char *big, const char *little,
						size_t len);
char				*ft_strrchr(const char *s, int c);
int					ft_tolower(int c);
int					ft_toupper(int c);
char				*ft_substr(char const *s, unsigned int start, size_t len);
char				*ft_strjoin(char const *s1, char const *s2);
char				*ft_strtrim(char const *s1, char const *set);
char				**ft_split(char const *s, char c);
char				*ft_itoa(int n);
char				*ft_strmapi(char const *s, char (*f)(unsigned int, char));
void				ft_striteri(char *s, void (*f)(unsigned int, char *));
void				ft_putchar_fd(char c, int fd);
void				ft_putstr_fd(char *s, int fd);
void				ft_putendl_fd(char *s, int fd);
void				ft_putnbr_fd(int n, int fd);
t_list				*ft_lstnew(void *content);
void				ft_lstadd_front(t_list **lst, t_list *new);
int					ft_lstsize(t_list *lst);
t_list				*ft_lstlast(t_list *lst);
void				ft_lstadd_back(t_list **lst, t_list *new);
void				ft_lstdelone(t_list *lst, void (*del)(void *));
void				ft_lstclear(t_list **lst, void (*del)(void *));
void				ft_lstiter(t_list *lst, void (*f)(void *));
t_list				*ft_lstmap(t_list *lst, void *(*f)(void *),
						void (*del)(void *));
void				ft_putstrl(char *str, int *length);
void				ft_putnbrl(long nb, int *length);
void				ft_putcharl(char ch, int *length);
void				ft_puthexl(unsigned int num, int *length, int isbig);
void				ft_putaddress(unsigned long num, int *length);
char				*get_next_line(int fd);
int					ft_strlength(char *str);
int					ft_str_with_new_line_length(char *str);
int					ft_new_line_detector(char *str);
char				*ft_strjoin_to_new_line(char *s1, char *s2);
void				ft_remove_first_line(char *str);
int					ft_printf(const char *str, ...);
int					ft_type_identifier(char ch);
void				ft_type_distributor_middleware(va_list args, char ch,
						int *length);

#endif