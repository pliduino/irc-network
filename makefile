CC = gcc
CFLAGS = -I src/ram_lib

DEPS = src/ram_lib/ram_lib.h
OBJ = src/ram_lib/ram_lib.o src/main.o

NAME = bin/main

# all: o
# 	gcc src/main.c -o main


%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

all: $(OBJ)
	$(CC) -o $(NAME) $^ $(CFLAGS)

run: all
	@./$(NAME)