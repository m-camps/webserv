TARGET = webserv

DEPD = deps/
OBJD = objs/
SRCD = src/
INCD = inc/

SRCS := Cgi.cpp Client.cpp Generator.cpp Location.cpp main.cpp Network.cpp Parse.cpp Server.cpp Exchange.cpp Request.cpp Respond.cpp ResponseDelete.cpp ResponseGet.cpp ResponsePost.cpp deleteFile.cpp ft_stoi.cpp isDirectory.cpp readFile.cpp splitLineWithStrtok.cpp
OBJS = $(SRCS:%.cpp=$(OBJD)%.o)
DEPS = $(SRCS:%.cpp=$(DEPD)%.d)

vpath %.hpp $(INCD) $(SRCD)/Exchange $(SRCD)/Exchange
vpath %.cpp $(SRCD) $(SRCD)/Exchange $(SRCD)/Exchange/Methods $(SRCD)/Utilities
vpath %.o $(OBJD)

HFLAGS 		=	$(addprefix -I, $(INCD))
DEPFLAGS 	=	-MT $@ -MMD -MP -MF $(DEPD)$*.d
HEADER		=	inc
CC			=	c++ 
CFLAGS		=	-pedantic -Wall -Werror -Wextra -std=c++98
RM			=	rm -rf

all: $(OBJD) $(DEPD) $(TARGET) 

$(OBJD):
	@mkdir -p $(OBJD)

$(DEPD):
	@mkdir -p $(DEPD)

$(OBJD)%.o: %.cpp $(DEPD)%.d
	$(CC) $(CFLAGS) $(HFLAGS) $(DEPFLAGS) -c $< -o $@

$(TARGET) : $(OBJS)
	$(CC) $(CFLAGS) -I $(HEADER) $^ -o $@

clean:
	$(RM) $(OBJD)
	$(RM) $(DEPD)

fclean: clean
	$(RM) $(TARGET)

re: fclean all	

.PHONY : all clean fclean re

$(DEPS) :
include $(wildcard $(DEPS))



