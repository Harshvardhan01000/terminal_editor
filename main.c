#include <asm-generic/errno-base.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

#include <sys/ioctl.h>
#include <sys/types.h>
#include <unistd.h>

#include <termios.h>

#include <ctype.h>

#include <errno.h>

#include "term_text_editor.h"

struct termios og_state;

void exit_raw_mode() {
  printf("\033[2J\033[H"); // NOTE::clear screen and move to home
  if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &og_state) == -1)
    TERM_ERR("Error tcsetattr : %d", errno);
}

void init_raw_mode() {

  if (tcgetattr(STDIN_FILENO, &og_state) == -1)
    TERM_ERR("Error tcgetattr : %d", errno);

  atexit(exit_raw_mode);

  struct termios raw = og_state;
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

int main() {
  // struct winsize ws;

  init_raw_mode();

  // if (ioctl(STDIN_FILENO, TIOCGWINSZ, &ws) != 0 &&
  //     ioctl(STDOUT_FILENO, TIOCGWINSZ, &ws) != 0 &&
  //     ioctl(STDERR_FILENO, TIOCGWINSZ, &ws) != 0) {
  //   fprintf(stderr, "ioctl() failed (%d): %s\n", errno, strerror(errno));
  //   return 0;
  // }
  // size_t height = ws.ws_row;
  // size_t width = ws.ws_col;
  // char *screen = malloc(height * width * sizeof(char));
  // pane display = {screen, height, width, height, width};
  //
  // init_screen(&display);
  //
  // start_editor(&display);

  // free(screen);
  while (1) {
    char c = '\0';
    if (read(STDIN_FILENO, &c, sizeof(c)) == -1 && errno != EAGAIN)
      TERM_ERR("read with errno:%d", errno);
    if (iscntrl(c))
      printf("%d\r\n", c);
    else
      printf("%d ( '%c')\r\n", c, c);

    if ('q' == c)
      break;
  }

  // while (1) {
  //   char c;
  //   read(STDIN_FILENO, &c, sizeof(c));
  //   if (iscntrl(c)) {
  //     printf("%d" NEW_LINE, c);
  //   } else {
  //     printf("%d ('%c')" NEW_LINE, c, c);
  //   }
  //   if (c == 'q')
  //     break;
  // }

  return 0;
}
