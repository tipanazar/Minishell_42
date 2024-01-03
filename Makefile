CC = cc
CFLAGS = -Wall -Werror -Wextra -g
LDFLAGS = -lreadline
RM = rm -f

NAME = minishell
LIBFTDIR = ./libft
LIBFT = $(LIBFTDIR)/libft.a
SRCDIR = srcs
OBJDIR = objs
SRCS = main.c \
exit.c    \
parcer/runner.c \
parcer/run_redirect.c \
parcer/run_heredoc.c \
parcer/run_pipe.c \
parcer/parce-utils.c \
parcer/get-token.c \
parcer/parser.c \
utils/utils.c	  \
utils/utils1.c	  \
utils/full_path.c \
parcer/quotes.c	\
builtins/export.c	\
builtins/echo.c \
builtins/builtins.c \
builtins/cd.c \

OBJS = $(patsubst %.c,$(OBJDIR)/%.o,$(SRCS))

all: $(NAME)

$(NAME): $(LIBFT) $(OBJS)
	@$(CC) $(OBJS) $(LIBFT) $(LDFLAGS) -o $(NAME)
	@echo "\033[0;32m✅---COMPILING IS DONE---✅\033[0m"

$(OBJDIR)/%.o: $(SRCDIR)/%.c
	@mkdir -p $(@D)
	@$(CC) $(CFLAGS) -c $< -o $@

$(LIBFT):
	@make -sC $(LIBFTDIR)

clean:
	@$(RM) $(OBJS)
	@make -sC $(LIBFTDIR) clean

fclean: clean
	@$(RM) $(NAME)
	@make -sC $(LIBFTDIR) fclean

re: fclean all

v:
	make re && valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes --suppressions="supp.supp" ./minishell
	