# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: bolcay <bolcay@student.42.fr>              +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/03/17 10:02:15 by bolcay            #+#    #+#              #
#    Updated: 2025/04/23 15:10:23 by bolcay           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

SRCS = main.c get_next_line/get_next_line.c get_next_line/get_next_line_utils.c envp/envp_utils.c \
		envp/envp.c deallocation.c signals/set_signal.c \
		tokens/tokenizer.c builtin/builtin_utils.c tokens/tokenizer_utils.c builtin/env.c \
		builtin/pwd.c builtin/echo.c builtin/export.c builtin/unset.c builtin/cd.c builtin/exit.c \
		execute/exe_main.c execute/heredoc.c execute/exe_with_token.c execute/run_commands.c \
		execute/heredoc_utils.c envp/envp_utils2.c tokens/handle_tokens.c \
		execute/pipe.c execute/pipe_utils.c parse_func/parsing.c tokens/validate_syntax.c \
		parse_func/parse_utils.c builtin/builtin_utils2.c builtin/builtin_utils3.c \
		garbage_collector/garbage_collector.c builtin/builtin_utils4.c builtin/builtin_utils5.c



OBJS = $(SRCS:.c=.o)

NAME = minishell
CC = cc
CFLAGS = -Wall -Werror -Wextra -g $(INCLUDE)
RM = rm -f
RM_DIR = rm -rf

LIBFT_DIR = libft
LIBFT_LIB = ${LIBFT_DIR}/libft.a

INCLUDE     = -I header -I libft
LDINCLUDE   = -L$(LIBFT_DIR) -lft

all: ${NAME}

$(LIBFT_LIB):
	make -C $(LIBFT_DIR) && make clean -C $(LIBFT_DIR)

$(NAME): $(LIBFT_LIB) $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) $(LDINCLUDE) -lreadline -o $(NAME)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean: 
	$(RM_DIR) $(OBJS)

fclean: clean
		$(RM) $(NAME)
		make fclean -C $(LIBFT_DIR)

re: fclean all

.PHONY: all clean fclean re