# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: bolcay <bolcay@student.42.fr>              +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/03/17 10:02:15 by bolcay            #+#    #+#              #
#    Updated: 2025/03/20 14:32:28 by bolcay           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

SRCS = main.c get_next_line/get_next_line.c get_next_line/get_next_line_utils.c envp/envp_utils.c \
		envp/envp.c deallocation.c builtin.c token.c signals/set_signal.c \
		tokens/tokenizer.c builtin_utils.c

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