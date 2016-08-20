#include <pebble.h>
#include "strdup.h"

char *strdup(const char *s)
{
  char *buf = malloc(strlen(s) + 1);
  if (buf) {
    strcpy(buf, s);
  }
  return buf;
}
