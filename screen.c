#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <string.h>
#include <unistd.h>

#include "term_text_editor.h"

// typedef struct screen {
//   char *screen_buffer;
//   size_t height;
//   size_t width;
// } pane;

extern struct editor_config E;

void editor_draw_rows(buf *ab) {
  size_t y;
  for (y = 0; y < E.row; y++) {
    ab_append(ab, "~", 1);
    if (y == E.row / 3) {
      size_t new_size = E.col - 1;
      char welcome[80];

      size_t welcomelen =
          snprintf(welcome, sizeof(welcome),
                   "HELLO TO MY VIM CLONE VERSION : %1.1f", __TERM_VERSION__);

      if (welcomelen > new_size)
        welcomelen = new_size;

      char title[new_size];

      memset(title, ' ', new_size);

      int mid = (new_size - welcomelen) / 2;

      memcpy(title + mid, welcome, welcomelen);

      ab_append(ab, title, new_size - 2);
    }

    ab_append(ab, "\x1b[K", 3);
    if (y < E.row - 1)
      ab_append(ab, "\r\n", 2);
  }
}

void editor_refresh_screen() {
  buf ab = ABUF_INIT;

  ab_append(&ab, "\x1b[?25l", 6);
  // ab_append(&ab, "\x1b[2J", 4);
  ab_append(&ab, "\x1b[H", 3);

  editor_draw_rows(&ab);

  char buf[32];
  snprintf(buf, sizeof(buf), "\x1b[%d;%dH", E.cy + 1, E.cx + 1);
  ab_append(&ab, buf, strlen(buf));
  ab_append(&ab, "\x1b[?25h", 6);
  write(STDOUT_FILENO, ab.b, ab.len);

  ab_free(&ab);
}

// void init_screen(pane *d) {
//   printf("\033[2J\033[H"); // NOTE::clear screen and move to home
//   size_t i = 0;
//   char *index = malloc(sizeof(char) * 22);
//   int index_width = snprintf(NULL, 0, "%zu", d->height);
//   int written_width = sprintf(index, "|%*zu.|", index_width, i);
//   while (i < d->height - 1) {
//     sprintf(index, "|%*zu.", index_width, i);
//
//     char *start = d->screen_buffer + (i * d->width);
//     memset(start, '-', d->width - 1);
//     strncpy(start, index, strlen(index));
//     i++;
//     *(d->screen_buffer + (i * d->width - 2)) = '|';
//     *(d->screen_buffer + (i * d->width - 1)) = '\n';
//   }
//   *(d->screen_buffer + (d->height * d->width - 1)) = '\0';
//
//   printf("%s", d->screen_buffer);
//   free(index);
//   fflush(stdout);
// }

// void start_editor(pane *d) {
//   printf("BEFORE\n");
//
//   printf("\033[1;1H");
//   printf("\033[?25h");
//
//   char c;
//   while (1) {
//     read(STDIN_FILENO, &c, sizeof(c));
//     printf("%c", c);
//     fflush(stdout);
//   }
// }
