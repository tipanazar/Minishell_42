YELLOW=\033[0;34m
COLOR_END = \033[0m
NAME = libft.a
SRC_FOLDER = srcs/
SRCS =  $(SRC_FOLDER)ft_isalnum.c      \
		$(SRC_FOLDER)ft_isprint.c      \
		$(SRC_FOLDER)ft_memcmp.c       \
		$(SRC_FOLDER)ft_strdup.c       \
		$(SRC_FOLDER)ft_substr.c       \
		$(SRC_FOLDER)ft_strlcat.c      \
		$(SRC_FOLDER)ft_isalpha.c      \
		$(SRC_FOLDER)ft_strlen.c 	   \
		$(SRC_FOLDER)ft_strrchr.c      \
		$(SRC_FOLDER)ft_memcpy.c       \
		$(SRC_FOLDER)ft_strchr.c 	   \
		$(SRC_FOLDER)ft_strlcpy.c 	   \
		$(SRC_FOLDER)ft_memmove.c      \
		$(SRC_FOLDER)ft_calloc.c       \
		$(SRC_FOLDER)ft_tolower.c      \
		$(SRC_FOLDER)ft_bzero.c 	   \
		$(SRC_FOLDER)ft_isascii.c      \
		$(SRC_FOLDER)ft_memcpy.c       \
		$(SRC_FOLDER)ft_strnstr.c      \
		$(SRC_FOLDER)ft_toupper.c      \
		$(SRC_FOLDER)ft_isdigit.c      \
		$(SRC_FOLDER)ft_memchr.c       \
		$(SRC_FOLDER)ft_memset.c       \
		$(SRC_FOLDER)ft_split.c 	   \
		$(SRC_FOLDER)ft_itoa.c 	       \
		$(SRC_FOLDER)ft_strmapi.c      \
		$(SRC_FOLDER)ft_striteri.c     \
		$(SRC_FOLDER)ft_putchar_fd.c   \
		$(SRC_FOLDER)ft_putstr_fd.c    \
		$(SRC_FOLDER)ft_strtrim.c      \
		$(SRC_FOLDER)ft_strjoin.c      \
		$(SRC_FOLDER)ft_putendl_fd.c   \
		$(SRC_FOLDER)ft_putnbr_fd.c    \
		$(SRC_FOLDER)ft_lstnew.c	   \
		$(SRC_FOLDER)ft_lstsize.c 	   \
		$(SRC_FOLDER)ft_lstiter.c 	   \
		$(SRC_FOLDER)ft_lstlast.c 	   \
		$(SRC_FOLDER)ft_lstmap.c 	   \
		$(SRC_FOLDER)ft_lstadd_back.c  \
		$(SRC_FOLDER)ft_lstdelone.c    \
		$(SRC_FOLDER)ft_lstclear.c	   \
		$(SRC_FOLDER)ft_lstadd_front.c \
		$(SRC_FOLDER)ft_strncmp.c      \
		$(SRC_FOLDER)ft_putstrl.c 	   \
		$(SRC_FOLDER)ft_putnbrl.c	   \
		$(SRC_FOLDER)ft_putcharl.c	   \
		$(SRC_FOLDER)ft_puthexl.c	   \
		$(SRC_FOLDER)ft_putaddress.c   \
		$(SRC_FOLDER)get_next_line.c   \
		$(SRC_FOLDER)get_next_line_utils.c   \
		$(SRC_FOLDER)ft_printf.c		   \
		$(SRC_FOLDER)ft_type_distributor_middleware.c   \
		$(SRC_FOLDER)ft_type_identifier.c	\
		$(SRC_FOLDER)ft_free_char_arr.c	\
		$(SRC_FOLDER)ft_char_arr_length.c	\

OBJS = $(SRCS:.c=.o)

all: $(NAME)

$(NAME): $(OBJS)
	@ar -crs $@ $^
	@rm -f $(OBJS)
	@echo "$(YELLOW)\n--Libft COMPILED--\n$(COLOR_END)"

clean:
	@rm -f $(OBJS)

fclean: clean
	@rm -f $(NAME)
	
re: fclean all

.PHONY: all clean fclean re bonus