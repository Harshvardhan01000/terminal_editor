#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

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
  fflush(stdout);
}

void start_editor(pane *d) {
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
