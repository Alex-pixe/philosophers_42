# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: cbridget <cbridget@student.21-school.ru    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/01/05 19:40:05 by cbridget          #+#    #+#              #
#    Updated: 2022/04/20 18:29:06 by cbridget         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = philo_bonus

HEAD = hdrs/philosophers_bonus.h

SRC = philosophers_bonus.c parser_bonus.c parser2_bonus.c \
ph_process_bonus.c

OBJ = $(addprefix obj/,$(SRC:.c=.o))

BUILD_FOLDER := $(shell mkdir -p obj)

FLAGS = -Wall -Werror -Wextra

CC = cc

.PHONY : all clean fclean re

all : $(NAME)

$(NAME) : $(OBJ)
	$(CC) $(FLAGS) $(OBJ) -o $(NAME)

obj/%.o : src/%.c $(HEAD) Makefile
	$(CC) $(FLAGS) -iquote hdrs -c $< -o $@

clean :
	rm -rf obj

fclean : clean
	rm -f $(NAME)

re : fclean
	@mkdir -p obj
	$(MAKE)
