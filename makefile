CC = gcc
CFLAGS = -I src/ram_lib
NAME = main

# Finds files matching wildcard recursively
rwildcard=$(wildcard $1$2) $(foreach d,$(wildcard $1*),$(call rwildcard,$d/,$2))

DEPS = $(subst /,\,$(call rwildcard,src/,*.h))
SRC = $(subst /,\,$(call rwildcard,src/,*.c))
OBJ = $(patsubst %.c,%.o, $(SRC))


ifeq ($(OS),Windows_NT)
	RM = del /q
# Supresses error messages from windows
	RME = 2>nul
else
    RM = rm -f
endif


%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

make: $(OBJ)
	$(CC) -o bin\$(NAME) $^ $(CFLAGS)

run: make
	@.\bin\$(NAME)

.PHONY: clean

clean:
	@ $(RM) bin\* $(OBJ) $(RME)