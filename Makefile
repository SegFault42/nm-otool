# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: rabougue <marvin@42.fr>                    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2015/12/10 19:16:51 by rabougue          #+#    #+#              #
#    Updated: 2017/01/18 21:47:46 by rabougue         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

####################################_COLOR_#####################################
GREEN = \033[38;5;10m
GREY = \033[38;5;60m
RED = \033[38;5;9m
END = \033[0m
##################################_COMPILATION_#################################
NAME = ft_nm
CC = clang
FLAG = -Weverything -g
LFT = ./libft/libft.a
SRC = ./source
SRCS = $(SRC)/main.c \
		$(SRC)/setup.c \

INCLUDE = -I ./include/ -I ./libft/includes/

OBJS = $(SRCS:.c=.o)

###########################_RELINK_MODIFY_.h####################################
RELINK_H = ./include/nm.h
################################################################################

###################################Compilation##################################
all: $(NAME)

$(NAME): $(OBJS)
	@make -s -C ./libft/
	@$(CC) $(LFT) $(OBJS) -o $(NAME)
	@printf "\n✅  Compilation done.\n"

%.o : %.c $(RELINK_H) ./Makefile
	@$(CC) -c $(FLAG) $< $(INCLUDE) -o $@

clean:
	@printf "                                                               \r"
	@printf "✅  all .o deleted\n"
	@rm -f $(OBJS)
	@make -s clean -C ./libft/

fclean:
	@printf "                                                               \r"
	@printf "✅  libft.a, all .o and $(NAME) deleted\n"
	@rm -f $(NAME) $(CHECKER) $(OBJS) $(OBJS_CHECKER)
	@make -s fclean -C ./libft/

re: fclean all
