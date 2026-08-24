#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

#include <sys/ioctl.h>
#include <sys/types.h>
#include <unistd.h>

#include <errno.h>
#include <string.h>

typedef struct screen {
  char *screen_buffer;
  size_t height;
  size_t width;
  size_t writable_height;
  size_t writable_width;
} pane;

void init_screen(pane *d) {
  system("clear");
  size_t i = 0;
  char *index = malloc(sizeof(char) * 22);
  int index_width = snprintf(NULL, 0, "%zu", d->height);
  int written_width = sprintf(index, "|%*zu.|", index_width, i);
  d->writable_width = written_width;
  d->writable_height = d->height;
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
  free(index);
}

void start_editor(pane *d) {}

int main() {
  struct winsize ws;
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
  printf("%s", screen);

  start_editor(&display);

  free(screen);
  return 0;
}
