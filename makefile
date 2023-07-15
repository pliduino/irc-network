CC = gcc
CFLAGS = -I src/ram_lib
NAME = main

# Finds files matching wildcard recursively
rwildcard=$(wildcard $1$2) $(foreach d,$(wildcard $1*),$(call rwildcard,$d/,$2))

DEPS = $(call rwildcard,src/,*.h)
SRC = $(call rwildcard,src/,*.c)
OBJ = $(patsubst %.c,%.o, $(SRC))


RM = rm -f


%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

make: $(OBJ)
	$(CC) -o bin/$(NAME) $^ $(CFLAGS)

run: make
	@./bin/$(NAME)

.PHONY: clean

clean:
	@ $(RM) bin/* $(OBJ) $(RME)
