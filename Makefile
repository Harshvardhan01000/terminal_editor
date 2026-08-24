CC = gcc

CFLAGS = -std=c17 -Wall -Wextra -Wpedantic -g \
         -fsanitize=address,undefined

TARGET = term_app
SOURCES = $(wildcard *.c)

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(SOURCES)
	$(CC) $(CFLAGS) $(SOURCES) -o $(TARGET)

clean:
	rm -f $(TARGET)
