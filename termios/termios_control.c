#include <stdio.h>
#include <sys/ioctl.h>
#include <unistd.h>

#include <stdlib.h>

#include <errno.h>

#include "../term_text_editor.h"

extern struct editor_config E;

void exit_raw_mode() {
  printf("\033[2J\033[H"); // NOTE:clear screen and move to home
  if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &E.og_state) == -1)
    TERM_ERR("Error tcsetattr : %d", errno);
}

void init_raw_mode() {

  if (tcgetattr(STDIN_FILENO, &E.og_state) == -1)
    TERM_ERR("Error tcgetattr : %d", errno);

  atexit(exit_raw_mode);

  struct termios raw = E.og_state;
  raw.c_iflag &=
      ~(BRKINT | ICRNL | INPCK | ISTRIP | IXON); // NOTE: following dogma
  raw.c_iflag &= ~(IXON | ICRNL);                // NOTE: turn of C-s and C-q
  raw.c_oflag &= ~(OPOST);                       // NOTE: turn of \n and \r\n
  raw.c_lflag &=
      ~(ICANON | ECHO | ISIG | IEXTEN); // NOTE: turn of C-c and C-z by ISIG

  raw.c_cc[VMIN] = 0;  // NOTE: set byte for input
  raw.c_cc[VTIME] = 1; // NOTE: set time for input

  if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw) == -1)
    TERM_ERR("Error tcsetattr : %d", errno);
}

int get_window_size(size_t *row, size_t *col) {

  struct winsize ws;

  if (ioctl(STDIN_FILENO, TIOCGWINSZ, &ws) != 0 &&
      ioctl(STDOUT_FILENO, TIOCGWINSZ, &ws) != 0 &&
      ioctl(STDERR_FILENO, TIOCGWINSZ, &ws) != 0)
    return -1;
  else {
    *row = ws.ws_row;
    *col = ws.ws_col;
    return 0;
  }
}

void init_editor() {
  init_raw_mode();
  if (get_window_size(&E.row, &E.col) == -1) {
    TERM_ERR("error get_window_size : %d", errno)
  }
}
