CC = gcc
CFLAGS = -Wall -Wextra -Iinclude
LIBS = -lmicrohttpd -lsqlite3
SRC = src/main.c src/server.c src/database.c src/base62.c
OBJ_DIR = build/obj
OBJ = $(SRC:src/%.c=$(OBJ_DIR)/%.o)
TARGET = build/curlshort

all: $(TARGET)

$(TARGET): $(OBJ)
	mkdir -p build
	$(CC) $(CFLAGS) -o $@ $^ $(LIBS)

$(OBJ_DIR)/%.o: src/%.c
	mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf build/