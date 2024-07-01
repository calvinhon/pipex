# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: chon <chon@student.42.fr>                  +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/06/10 11:20:21 by chon              #+#    #+#              #
#    Updated: 2024/06/19 15:37:03 by chon             ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = pipex
CFLAGS = -Wall -Werror -Wextra -fsanitize=address -g3
LIBFT_DIR = ./libft
LIBFT = ./libft/libft.a
OBJS = $(SRCS:.c=.o)
SRCS = main.c utils_1.c
OBJS_B = $(SRCS_B:.c=.o)
SRCS_B = ./bonus/main_bonus.c ./bonus/utils_1_bonus.c

$(NAME): $(OBJS)
	make -C $(LIBFT_DIR)
	$(CC) $(CFLAGS) $(OBJS) $(LIBFT) -o $(NAME)

all: $(NAME)

bonus: $(OBJS_B)
	make -C $(LIBFT_DIR)
	$(CC) $(CFLAGS) $(OBJS_B) $(LIBFT) -o $(NAME)

clean:
	make clean -C $(LIBFT_DIR)
	rm -f $(OBJS)
	rm -f $(OBJS_B)

fclean: clean
	make fclean -C $(LIBFT_DIR)
	rm -f $(NAME)

re: fclean all

.PHONY = all clean fclean re