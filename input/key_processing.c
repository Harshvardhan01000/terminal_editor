#include <errno.h>
#include <stddef.h>
#include <unistd.h>

#include "../term_text_editor.h"

extern struct editor_config E;

char editor_read_key() {
  int nread;
  char c;

  while ((nread = read(STDIN_FILENO, &c, sizeof(char))) == -1) {
    if (nread == -1 && errno != EAGAIN)
      TERM_ERR("error in read : %d", errno);
  }
  return c;
}

void editor_move_cursor(char c) {
  switch (c) {
  case 'h':
    if (E.cx > 0)
      E.cx--;
    break;
  case 'j':
    if ((size_t)E.cy < E.row)
      E.cy++;
    break;
  case 'k':
    if (E.cy > 0)
      E.cy--;
    break;
  case 'l':
    if ((size_t)E.cx < E.col)
      E.cx++;
    break;
  }
}

void editor_process_key() {
  char c = editor_read_key();
  switch (c) {
  case CTRL_KEY('q'):
    RETURN_TO_MAIN_BUFFER
    RESTORE_CURSOR
    exit(0);
    break;
  case 'h':
  case 'j':
  case 'k':
  case 'l':
    editor_move_cursor(c);
    editor_refresh_screen();
    break;
  }
}
