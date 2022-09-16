# **************************************************************************** #
#                                                                              #
#                                                         ::::::::             #
#    Makefile                                           :+:    :+:             #
#                                                      +:+                     #
#    By: mcamps <mcamps@student.codam.nl>             +#+                      #
#                                                    +#+                       #
#    Created: 2022/09/13 17:00:28 by mcamps        #+#    #+#                  #
#    Updated: 2022/09/16 17:19:24 by mcamps        ########   odam.nl          #
#                                                                              #
# **************************************************************************** #

NAME = webserv

SRC = src/main.cpp

CLIENT = client/main.cpp

INC = src/Config.hpp
	
OBJ = ${SRC:%.cpp=%.o}

CONFIG = config.conf

FLAGS = -Wall -Werror -Wextra
GCC = c++
EXTRA = -std=c++98
DEBUG = -fsanitize=address

%.o, %.hpp: %.cpp %.hpp
	$(GCC) -c -o $@ $< $(FLAGS) $(EXTRA) 
	
$(NAME): $(OBJ)
	$(GCC) $(OBJ) -o $(NAME) $(FLAGS) $(EXTRA) -I src/Config.hpp

all: $(NAME)
	@echo "$(GREEN)Compilation Complete $(COL_END)"

test: all
	@echo "$(YELLOW)Running Default || $(CONFIG) $(COL_END)"
	./$(NAME) $(CONFIG)

debug: 
	@$(GCC) $(OBJ) -o $(NAME) $(FLAGS) $(EXTRA) $(DEBUG)
	@echo "$(GREEN)Debug Compilation Complete $(COL_END)"
	@echo "$(PURPLE)Running Debug Mode || $(CONFIG)$(COL_END)"
	./$(NAME) $(CONFIG)

client:
	$(GCC) client/main.cpp -o not_server $(FLAGS) $(EXTRA)
	./not_server
	
clean:
	rm -rf $(OBJ)

fclean: clean
	rm -rf $(NAME)

re: fclean all

.PHONY: all test clean client $(NAME)

# Colors
RED=\033[1;31m
GREEN=\033[1;32m
PURPLE=\033[1;35m
YELLOW=\033[1;33m
COL_END=\033[0m