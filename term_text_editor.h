#ifndef TERM_TEXT_EDITOR
#define TERM_TEXT_EDITOR 1

#include "buffer.h"
#include <stdio.h>
#include <stdlib.h>
#include <termios.h>
#include <unistd.h>

#define __TERM_VERSION__ 0.1
#define TERM_ERR(message, ...)                                                 \
  do {                                                                         \
    REFRESH_SCREEN                                                             \
    fprintf(stderr, message "\n", __VA_ARGS__);                                \
    exit(1);                                                                   \
  } while (0);

#define NEW_LINE "\r\n"

#define REFRESH_SCREEN                                                         \
  do {                                                                         \
    write(STDOUT_FILENO, "\x1b[?1049h", 8);                                    \
  } while (0);

#define CURSOR_TOP                                                             \
  do {                                                                         \
    write(STDOUT_FILENO, "\x1b[H", 3);                                         \
  } while (0);

#define SAVE_CURSOR write(STDOUT_FILENO, "\0337", 2);

#define RESTORE_CURSOR                                                         \
  do {                                                                         \
    write(STDOUT_FILENO, "\0338", 2);                                          \
    write(STDOUT_FILENO, "\n", 1);                                             \
  } while (0);

#define RETURN_TO_MAIN_BUFFER write(STDOUT_FILENO, "\033[?1049l", 8);

#define CTRL_KEY(k) ((k) & 0x1f)

struct editor_config {
  int cx, cy;
  size_t row;
  size_t col;
  struct termios og_state;
};

void editor_refresh_screen();

#endif /* terminal.h  */
