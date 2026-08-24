#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

#include <sys/ioctl.h>
#include <sys/types.h>
#include <unistd.h>

#include <termios.h>

#include <errno.h>
#include <string.h>

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
}

void start_editor(pane *d) {
  // while (1) {
  char c;

  read(STDIN_FILENO, &c, 1);
  strncpy(d->screen_buffer + (d->height * d->width), &c, 1);

  printf("\033[H"); // move cursor to home
  printf("%s", d->screen_buffer);
  fflush(stdout);
  // }
}

int main() {
  struct winsize ws;
  struct termios raw;
  struct termios last_state;

  tcgetattr(STDIN_FILENO, &raw);
  last_state = raw;
  raw.c_lflag &= ~(ICANON | ECHO);
  tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);

  if (ioctl(STDIN_FILENO, TIOCGWINSZ, &ws) != 0 &&
      ioctl(STDOUT_FILENO, TIOCGWINSZ, &ws) != 0 &&
      ioctl(STDERR_FILENO, TIOCGWINSZ, &ws) != 0) {
    fprintf(stderr, "ioctl() failed (%d): %s\n", errno, strerror(errno));
    return 0;
  }
  size_t height = ws.ws_row;
  size_t width = ws.ws_col;
  char *screen = malloc(height * width * sizeof(char));
  pane display = {screen, height, width, height, width};

  init_screen(&display);

  start_editor(&display);

  free(screen);

  tcsetattr(STDIN_FILENO, TCSAFLUSH, &last_state);
  return 0;
}
