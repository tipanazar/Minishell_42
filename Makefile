CC = cc
CFLAGS = -Wall -Werror -Wextra
RM = rm -f

EXECUTABLE = minishell
COLOR_GREEN=\033[0;32m
COLOR_END = \033[0m
LIBFTDIR = ./libft
LIBFT = $(LIBFTDIR)/libft.a

SRCS = main.c
OBJS = $(addprefix obj/, $(SRCS:.c=.o))

all: $(EXECUTABLE)

$(EXECUTABLE): $(OBJS) $(LIBFT)
	@$(CC) $(CFLAGS) $(OBJS) $(LIBFT) -o $(EXECUTABLE)
	@echo "$(COLOR_GREEN)\n✅---COMPILING IS DONE---✅\n$(COLOR_END)"

obj/%.o: %.c
	@mkdir -p obj
	@$(CC) $(CFLAGS) -c $< -o $@

$(LIBFT):
	@make -sC $(LIBFTDIR)

clean:
	@$(RM) $(OBJS)
	@make -sC $(LIBFTDIR) clean

fclean: clean
	@$(RM) $(EXECUTABLE)
	@rm -rf obj

re: fclean all

.PHONY: all clean fclean re
