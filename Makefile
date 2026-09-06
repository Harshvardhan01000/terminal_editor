CC = gcc

CFLAGS = -std=c17 -Wall -Wextra -Wpedantic -g \
         -fsanitize=address,undefined

TARGET = binary

SOURCES := $(shell find . -type f -name '*.c')
HEADERS := $(shell find . -type f -name '*.h')

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(SOURCES) $(HEADERS)
	$(CC) $(CFLAGS) $(SOURCES) -o $(TARGET)

clean:
	rm -f $(TARGET)
