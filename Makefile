# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: hpehliva <hpehliva@student.42heilbronn.    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/03/17 10:02:15 by bolcay            #+#    #+#              #
#    Updated: 2025/04/28 23:14:17 by hpehliva         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

SRCS = main.c get_next_line/get_next_line.c get_next_line/get_next_line_utils.c envp/envp_utils.c \
		envp/envp.c signals/set_signal.c \
		tokens/tokenizer.c builtin/builtin_utils.c tokens/tokenizer_utils.c builtin/env.c \
		builtin/pwd.c builtin/echo.c builtin/export.c builtin/unset.c builtin/cd.c builtin/exit.c \
		execute/exe_main.c execute/heredoc.c execute/exe_with_token.c execute/run_commands.c \
		execute/heredoc_utils.c envp/envp_utils2.c tokens/handle_tokens.c \
		execute/pipe.c execute/pipe_utils.c parse_func/parsing.c tokens/validate_syntax.c \
		parse_func/parse_utils.c builtin/builtin_utils2.c builtin/builtin_utils3.c \
		garbage_collector/garbage_collector.c tokens/handle_tokens1.c tokens/handle_tokens2.c \
		tokens/handle_tokens3.c tokens/token_heredoc.c tokens/token_expansion.c tokens/tokenizer_utils1.c \
		builtin/builtin_utils4.c builtin/builtin_utils5.c execute/exe_utils.c execute/exe_utils1.c \
		tokens/tokenizer_utils2.c signals/set_signal1.c parse_func/parse_utils1.c parse_func/parse_utils2.c \
		parse_func/parse_utils3.c garbage_collector/garbage_collector1.c builtin/cd_helper.c execute/pipe_utils1.c \
		execute/exe_utils2.c execute/exe_utils3.c execute/exe_with_token2.c envp/envp_utils3.c envp/envp_utils4.c \
		envp/envp_utils5.c




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