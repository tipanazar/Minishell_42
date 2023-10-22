CC = cc
CFLAGS = -Wall -Werror -Wextra
LDFLAGS = -lreadline
RM = rm -f

NAME = minishell
LIBFTDIR = ./libft
LIBFT = $(LIBFTDIR)/libft.a
SRCS = srcs/main.c \
srcs/parcer/commands-runner.c \
srcs/parcer/commands-utils.c \
srcs/parcer/get-token.c \
srcs/parcer/parcers.c \
srcs/utils/utils1.c	  \
srcs/utils/ft_fileno.c \

OBJS = $(SRCS:.c=.o)

all: $(NAME)

$(NAME): $(LIBFT) $(OBJS)
	@$(CC) $(OBJS) $(LIBFT) $(LDFLAGS) -o $(NAME)
	@echo "\033[0;32m✅---COMPILING IS DONE---✅\033[0m"

$(LIBFT):
	@make -sC $(LIBFTDIR)

clean:
	@$(RM) $(OBJS)
	@make -sC $(LIBFTDIR) clean

fclean: clean
	@$(RM) $(NAME)
	@make -sC $(LIBFTDIR) fclean

re: fclean all
