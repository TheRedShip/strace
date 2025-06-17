# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: TheRed <TheRed@students.42.fr>             +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/06/05 15:40:30 by TheRed            #+#    #+#              #
#    Updated: 2025/06/05 15:40:30 by TheRed           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

BLACK		=	\033[30;49;3m
RED			=	\033[31;49;3m
GREEN		=	\033[32;49;3m
YELLOW		=	\033[33;49;3m
BLUE		=	\033[34;49;3m
MAGENTA		=	\033[35;49;3m
CYAN		=	\033[36;49;3m
WHITE		=	\033[37;49;3m

BBLACK		=	\033[30;49;3;1m
BRED		=	\033[31;49;3;1m
BGREEN		=	\033[32;49;3;1m
BYELLOW		=	\033[33;49;3;1m
BBLUE		=	\033[34;49;3;1m
BMAGENTA	=	\033[35;49;3;1m
BCYAN		=	\033[36;49;3;1m
BWHITE		=	\033[37;49;3;1m

RESET		=	\033[0m

LINE_CLR	=	\33[2K\r

FILE		=	$(shell ls -lR srcs/ | grep -F .c | wc -l)
CMP			=	1

NAME        :=	ft_strace

SRCS_DIR	:=	srcs

OBJS_DIR	:=	.objs

ALL_SRCS	:=	main.c				\
				utils.c				\
				parsing.c			\
				syscall.c			\
				print.c				\
				signal.c			\


SRCS		:=	$(ALL_SRCS:%=$(SRCS_DIR)/%)


OBJS		:=	$(addprefix $(OBJS_DIR)/, $(SRCS:%.c=%.o))

HEADERS		:=	includes/strace.h

CC          :=	clang

CFLAGS      :=	-Wall -Wextra -Werror -g

IFLAGS	    :=	-I ./includes


RM          :=	rm -rf

MAKEFLAGS   += --no-print-directory

DIR_DUP     =	mkdir -p $(@D)

# RULES ********************************************************************** #

all: $(NAME)

bonus: all

$(NAME): $(OBJS) $(HEADERS)
	@$(CC) $(CFLAGS) $(IFLAGS) $(OBJS) -lm -o $(NAME)
	@printf "$(LINE_CLR)$(BWHITE) $(NAME): PROJECT COMPILED !$(RESET)\n\n"

$(OBJS_DIR)/%.o: %.c
	@$(DIR_DUP)
	@if [ $(CMP) -eq '1' ]; then \
		printf "\n"; \
	fi;
	@printf "$(LINE_CLR)$(WHITE) $(NAME): $(CMP)/$(FILE) $(BWHITE)$<$(RESET) $(GREEN)compiling...$(RESET)"
	@$(CC) $(CFLAGS) $(IFLAGS) -o $@ -c $^
	@$(eval CMP=$(shell echo $$(($(CMP)+1))))
	@if [ $(CMP) -gt $(FILE) ]; then \
		printf "$(LINE_CLR)$(WHITE) $(NAME): $$(($(CMP)-1))/$(FILE)\n$(LINE_CLR)$(BGREEN) Compilation done !$(RESET)\n"; \
	fi \


clean:
	@$(RM) $(OBJS)

dclean: clean
	@$(RM) $(OBJS_DIR)

fclean: dclean
	@printf " $(BWHITE)$(NAME):$(BRED) cleaned.$(RESET)\n"
	@$(RM) $(NAME)

re:
	@$(MAKE) fclean
	@$(MAKE) all

test:
	@${CC} -o test test.c

# **************************************************************************** #

.PHONY: all clean fclean dclean re bonus