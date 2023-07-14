CC = gcc
CFLAGS = -I src/ram_lib
NAME = main

rwildcard=$(wildcard $1$2) $(foreach d,$(wildcard $1*),$(call rwildcard,$d/,$2))

DEPS = $(call rwildcard,src/,*.h)
SRC = $(call rwildcard,src/,*.c)
OBJ = $(patsubst %.c,%.o, $(SRC))


ifeq ($(OS),Windows_NT)
	RM = del /q
else
    RM = rm -f
endif


%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

make: $(OBJ)
	$(CC) -o bin/$(NAME) $^ $(CFLAGS)

run: make
	@./bin/$(NAME)

.PHONY: clean

clean:
	$(RM) bin\* $(OBJ)