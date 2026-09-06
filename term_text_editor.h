#ifndef TERM_TEXT_EDITOR
#define TERM_TEXT_EDITOR 1

#include <stdio.h>
#include <stdlib.h>
#include <termios.h>
#include <unistd.h>

#define TERM_ERR(message, ...)                                                 \
  do {                                                                         \
    REFRESH_SCREEN                                                             \
    fprintf(stderr, message "\n", __VA_ARGS__);                                \
    exit(1);                                                                   \
  } while (0);

#define NEW_LINE "\r\n"

#define REFRESH_SCREEN                                                         \
  do {                                                                         \
    write(STDOUT_FILENO, "\x1b[2J", 4);                                        \
    write(STDOUT_FILENO, "\x1b[H", 3);                                         \
  } while (0);

#define CURSOR_TOP                                                             \
  do {                                                                         \
    write(STDOUT_FILENO, "\x1b[H", 3);                                         \
  } while (0);

#define CTRL_KEY(k) ((k) & 0x1f)

struct editor_config {
  size_t row;
  size_t col;
  struct termios og_state;
};

void editor_draw_rows(size_t row);

#endif /* terminal.h  */
