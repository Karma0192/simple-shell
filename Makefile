CC     = gcc
CFLAGS = -Wall -Wextra -pedantic -Iinclude
TARGET = mysh
SRC    = src/main.c src/shell.c src/builtins.c src/parser.c

all: $(TARGET)

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) -o $(TARGET) $(SRC)

clean:
	rm -f $(TARGET)

.PHONY: all clean