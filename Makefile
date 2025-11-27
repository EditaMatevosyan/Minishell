NAME = minishell

CC = cc
CFLAGS = -Wall -Wextra -Werror -Iinc #-g3 -fsanitize=thread

HEADERS = inc/minishell.h

SRCS_DIR = src/
OBJS_DIR = objs/

SRCS_NAME = main.c \
			tokenization.c \
			tokenize_input.c \
			tokenize_quotes.c \
			libft_ut1.c \
			libft_ut2.c \
			libft_ut3.c \
			copy_env.c \
			tokenize_env.c \
			expand_tokens.c \
			ft_itoa.c \
			ft_split.c \
			temp.c \
			redirections.c \
			execute.c \
			execution_utils.c \
			syntax_validation.c \
			built_in.c \
			echo.c \
			pwd.c

OBJS_NAME = $(SRCS_NAME:.c=.o)
OBJS = $(addprefix $(OBJS_DIR), $(OBJS_NAME))

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $^ -o $@ -lreadline -L/usr/lib -I/usr/include

$(OBJS_DIR):
	mkdir -p $(OBJS_DIR)

$(OBJS_DIR)%.o: $(SRCS_DIR)%.c $(HEADERS) Makefile | $(OBJS_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	$(RM) $(OBJS)

fclean: clean
	$(RM) $(NAME)

re: fclean all

.PHONY: all clean fclean re