#include <errno.h>
#include <unistd.h>

#include "../term_text_editor.h"

char editor_read_key() {
  int nread;
  char c;

  while ((nread = read(STDIN_FILENO, &c, sizeof(char))) == -1) {
    if (nread == -1 && errno != EAGAIN)
      TERM_ERR("error in read : %d", errno);
  }
  return c;
}

void editor_process_key() {
  char c = editor_read_key();
  switch (c) {
  case CTRL_KEY('q'):
    RETURN_TO_MAIN_BUFFER
    RESTORE_CURSOR
    exit(0);
    break;
  }
}
