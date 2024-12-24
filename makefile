PROJECT = libftpp
CC = c++
FLAGS = -Wall -Wextra -Werror -std=c++23 -O3
SRCDIR = src
INCDIR = inc
OBJDIR = obj

SRCS := $(wildcard $(SRCDIR)/*.cpp)
OBJS := $(patsubst $(SRCDIR)/%.cpp, $(OBJDIR)/%.o, $(SRCS))

$(OBJDIR)/%.o: $(SRCDIR)/%.cpp
	$(CC) $(FLAGS) -I$(INCDIR) -c $< -o $@

all: $(OBJS)
	ar rcs $(PROJECT).a $(OBJS)
	$(CC) $(FLAGS) test.cpp -I$(INCDIR) -L. -lftpp -o $(PROJECT)

clean:
	rm -rf $(OBJDIR)/*

fclean: clean
	rm -f $(PROJECT)

re: fclean all

.PHONY:
	all clean fclean re
