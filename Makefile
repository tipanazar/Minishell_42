CC = cc
CFLAGS =  -Wall -Werror -Wextra -lreadline
RM = rm -f

NAME = minishell
COLOR_GREEN=\033[0;32m
COLOR_END = \033[0m
LIBFTDIR = ./libft
LIBFT = $(LIBFTDIR)/libft.a
SRCS = srcs/*.c 
	
all: $(NAME)

$(NAME): $(LIBFT) 
	@cp $(LIBFT) $@
	@$(CC) $(CFLAGS) $(SRCS) $(LIBFT) -o $(NAME)
	@echo "$(COLOR_GREEN)\n✅---COMPILING IS DONE---✅\n$(COLOR_END)"

$(LIBFT):
	@make -sC $(LIBFTDIR)

clean:
	@$(RM) $(LIBFT)
	@make -sC $(LIBFTDIR) clean

fclean: clean
	@$(RM) $(NAME)

re: fclean all