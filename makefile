CC = gcc
CFLAGS = -I src/ram_lib

DEPS = src\ram_lib\ram_lib.h
OBJ = src\ram_lib\ram_lib.o src\main.o

NAME = main

ifeq ($(OS),Windows_NT)
	RM = del /q
    RRM = rmdir /q /s
else
    RM = rm -f
    RRM = rm -f -r
endif

# all: o
# 	gcc src/main.c -o main


%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

make: $(OBJ)
	$(CC) -o bin/$(NAME) $^ $(CFLAGS)

run: make
	@./bin/$(NAME)

.PHONY: clean

clean:
	@ $(RM) bin\* $(OBJ)