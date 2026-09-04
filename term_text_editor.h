#ifndef TERM_TEXT_EDITOR
#define TERM_TEXT_EDITOR 1

#define TERM_ERR(message, ...)                                                 \
  do {                                                                         \
    fprintf(stderr, message "\n", __VA_ARGS__);                                \
    exit(1);                                                                   \
  } while (0)

#define NEW_LINE "\r\n"

#endif /* terminal.h  */
