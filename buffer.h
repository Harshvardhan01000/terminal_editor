#ifndef BUFFER_H
#define BUFFER_H

typedef struct abuf {
  char *b;
  int len;
} buf;

#define ABUF_INIT {NULL, 0}

void ab_append(buf *ab, const char *s, int len);

void ab_free(buf *ab);

#endif
