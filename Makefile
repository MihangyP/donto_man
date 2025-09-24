NAME = donto_man
SRCS = main.c
SRC_FILES = $(addprefix src/, $(SRCS))
OBJ_FILES = $(SRC_FILES:.c=.o)
HEADERS = donto_man.h
HEADER_FILES = $(addprefix include/, $(HEADERS))
GDB ?=
CC = cc
CFLAGS = $(GDB) -Wall -Wextra #-Werror
RM = rm -rf
INCLUDING = -I./include
LINKING = -lX11 

all: $(NAME)

$(NAME): $(SRC_FILES) $(HEADER_FILES)
	$(CC) $(CFLAGS) $(INCLUDING) $(SRC_FILES) -o $(NAME) $(LINKING)

clean:
	$(RM) $(OBJ_FILES)

fclean: clean
	$(RM) $(NAME)

re: fclean all

.PHONY: all clean fclean re
