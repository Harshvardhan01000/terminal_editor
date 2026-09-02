#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

#include <sys/ioctl.h>
#include <sys/types.h>
#include <unistd.h>

#include <termios.h>

#include <ctype.h>

#include <errno.h>
#include <string.h>

#define TERM_ERR(message, ...) fprintf(stderr, message "\n", __VA_ARGS__)

#define NEW_LINE "\r\n"

struct termios og_state;

typedef struct screen {
  char *screen_buffer;
  size_t height;
  size_t width;
} pane;

void init_screen(pane *d) {
  printf("\033[2J\033[H"); // NOTE::clear screen and move to home
  size_t i = 0;
  char *index = malloc(sizeof(char) * 22);
  int index_width = snprintf(NULL, 0, "%zu", d->height);
  int written_width = sprintf(index, "|%*zu.|", index_width, i);
  while (i < d->height - 1) {
    sprintf(index, "|%*zu.", index_width, i);

    char *start = d->screen_buffer + (i * d->width);
    memset(start, '-', d->width - 1);
    strncpy(start, index, strlen(index));
    i++;
    *(d->screen_buffer + (i * d->width - 2)) = '|';
    *(d->screen_buffer + (i * d->width - 1)) = '\n';
  }
  *(d->screen_buffer + (d->height * d->width - 1)) = '\0';

  printf("%s", d->screen_buffer);
  free(index);
  fflush(stdout);
}

void start_editor(pane *d) {
  (void *)d;
  printf("BEFORE\n");

  printf("\033[1;1H");
  printf("\033[?25h");

  char c;
  while (1) {
    read(STDIN_FILENO, &c, sizeof(c));
    printf("%c", c);
    fflush(stdout);
  }
}

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
    char c;
    read(STDIN_FILENO, &c, sizeof(c));
    if (iscntrl(c)) {
      printf("%d" NEW_LINE, c);
    } else {
      printf("%d ('%c')" NEW_LINE, c, c);
    }
    if (c == 'q')
      break;
  }

  return 0;
}
