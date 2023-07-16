CC := gcc
CFLAGS := -Wall

MKDIR := mkdir -p
RMDIR := rm -rf

SRC_DIR := src
LIB_DIR := lib
OUT_DIR := out

CLIENT_MAIN_FILE := $(SRC_DIR)/client.c
SERVER_MAIN_FILE := $(SRC_DIR)/server.c

CLIENT_BIN_FILE := $(OUT_DIR)/client.bin
SERVER_BIN_FILE := $(OUT_DIR)/server.bin

LIB_FILES := $(shell find . -wholename './$(LIB_DIR)/*.c')

all: client server

client:
	$(MKDIR) $(OUT_DIR)
	$(CC) $(CFLAGS) -o $(CLIENT_BIN_FILE) $(CLIENT_MAIN_FILE) $(LIB_FILES)

server:
	$(MKDIR) $(OUT_DIR)
	$(CC) $(CFLAGS) -o $(SERVER_BIN_FILE) $(SERVER_MAIN_FILE) $(LIB_FILES)

clean:
	$(RMDIR) $(OUT_DIR)

run-client: client
	$(CLIENT_BIN_FILE)

run-server: server
	$(SERVER_BIN_FILE)

valgrind-client: client
	valgrind $(CLIENT_BIN_FILE)

valgrind-server: server
	valgrind $(SERVER_BIN_FILE)