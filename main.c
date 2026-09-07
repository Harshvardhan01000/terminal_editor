#include "input/key_processing.h"
#include "term_text_editor.h"
#include "termios/termios_control.h"

struct editor_config E;
int main() {
  // struct winsize ws;

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
  init_editor();
  SAVE_CURSOR
  REFRESH_SCREEN
  editor_draw_rows(E.row);
  CURSOR_TOP
  while (1)
    editor_process_key();

  return 0;
}
