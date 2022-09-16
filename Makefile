# **************************************************************************** #
#                                                                              #
#                                                         ::::::::             #
#    Makefile                                           :+:    :+:             #
#                                                      +:+                     #
#    By: mcamps <mcamps@student.codam.nl>             +#+                      #
#                                                    +#+                       #
#    Created: 2022/09/13 17:00:28 by mcamps        #+#    #+#                  #
#    Updated: 2022/09/13 17:11:15 by mcamps        ########   odam.nl          #
#                                                                              #
# **************************************************************************** #

NAME = webserv

SRC = $(shell find . -type f -name "*.cpp")

OBJ = ${SRC:%.cpp=%.o}

CONFIG = config.conf

CFLAGS = -Wall -Werror -Wextra -pedantic
GCC	= c++
EXTRA = -std=c++98
SHELL := /bin/bash

ifdef DEBUG
	CFLAGS += -g
else ifdef FSAN
	CFLAGS += -fsanitize=address
else
	CFLAGS += -Ofast
endif

%.o:%.cpp %.hpp
	$(GCC) -c -o $@ $< $(CFLAGS) $(EXTRA)
	
$(NAME): $(OBJ)
	$(GCC) $(OBJ) -o $(NAME) $(CFLAGS) $(EXTRA)

all: $(NAME)
	@printf "$(GREEN)Compilation Complete $(COL_END)"

test: all
	@printf "$(YELLOW)Running Default || $(CONFIG) $(COL_END)"
	./$(NAME) $(CONFIG)

debug:
	make DEBUG=1
	@printf "$(GREEN)Debug Compilation Complete $(COL_END)"
	@printf "$(PURPLE)Running Debug Mode || $(CONFIG)$(COL_END)"

rebug: fclean
	make debug

clean:
	rm -rf $(OBJ)

fclean: clean
	rm -rf $(NAME)

re: fclean all

fsan:
	make FSAN=1

resan: fclean
	make fsan

run: all
	./$(NAME)

rerun: fclean all
	./$(NAME)

# Colors
RED=\x1b[1;31m
GREEN=\x1b[32;01m
PURPLE=\033[1;35m
YELLOW=\033[1;33m
COL_END=\x1b[0m

.PHONY:	all clean fclean re