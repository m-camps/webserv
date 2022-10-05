# **************************************************************************** #
#                                                                              #
#                                                         ::::::::             #
#    Makefile                                           :+:    :+:             #
#                                                      +:+                     #
#    By: mcamps <mcamps@student.codam.nl>             +#+                      #
#                                                    +#+                       #
#    Created: 2022/09/13 17:00:28 by mcamps        #+#    #+#                  #
#    Updated: 2022/10/05 11:14:09 by mcamps        ########   odam.nl          #
#                                                                              #
# **************************************************************************** #

NAME := webserv

SRCDIR := src
BUILDDIR := build

SRC := $(shell find $(SRCDIR) -type f -name "*.cpp")
OBJ := $(patsubst $(SRCDIR)/%,$(BUILDDIR)/%,$(SRC:.cpp=.o))

CONFIG := config.conf

CFLAGS := -Wall -Werror -Wextra -pedantic
CC	:= c++
EXTRA := -std=c++98
SHELL := /bin/bash
INC := -Iinc -Isrc

ifdef DEBUG
	CFLAGS += -g
else ifdef FSAN
	CFLAGS += -fsanitize=address
else
	CFLAGS += -Ofast
endif

$(BUILDDIR)/%.o: $(SRCDIR)/%.cpp
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c -o $@ $<

$(NAME): $(OBJ)
	$(CC) $(CFLAGS) $^ -o $(NAME)

all: $(NAME)
	@printf "$(GREEN)Compilation Complete$(COL_END)"

# For LLDB or any other debugger
debug:
	make DEBUG=1
	@printf "$(GREEN)Debug Compilation Complete $(COL_END)"
	@printf "$(PURPLE)Running Debug Mode || $(CONFIG)$(COL_END)"

rebug: fclean
	make debug

client:
	$(GCC) client/main.cpp -o not_server $(FLAGS) $(EXTRA)
	./not_server
	
clean:
	rm -rf $(BUILDDIR)

fclean: clean
	rm -rf $(NAME)

re: fclean all

#For segfaults
fsan:
	make FSAN=1

resan: fclean
	make fsan

#Make & run executable
run: all
	@printf "$(YELLOW)Running Default || $(CONFIG) $(COL_END)"
	./$(NAME) $(CONFIG)

rerun: fclean all
	@printf "$(YELLOW)Running Default || $(CONFIG) $(COL_END)"
	./$(NAME) $(CONFIG)

# Colors
RED=\x1b[1;31m
GREEN=\x1b[32;01m
PURPLE=\033[1;35m
YELLOW=\033[1;33m
COL_END=\x1b[0m

.PHONY:	all clean fclean re