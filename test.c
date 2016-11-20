#include <assert.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#define PEBBLE_V_TEST
#include "include/v.h"
#include "src/c/v.c"

static int intcmp(void *a, void *b) {
  return memcmp(a, b, sizeof(int));
}

int main(int argc, const char *const argv[]) {
  V thing;

  int buf;
  v_init(&thing, sizeof(buf));
  buf = 0;
  assert(0 == v_add(&thing, &buf));
  assert(!intcmp(&buf, v_get(&thing, 0)));
  buf = 1;
  assert(0 == v_add(&thing, &buf));
  assert(!intcmp(&buf, v_get(&thing, 1)));

  assert(1 == v_find(&thing, intcmp, &buf));
  buf = 0;
  assert(0 == v_find(&thing, intcmp, &buf));
  buf = 2;
  assert(-1 == v_find(&thing, intcmp, &buf));

  v_compact(&thing);

  buf = 0;
  assert(!intcmp(&buf, v_get(&thing, 0)));
  buf = 1;
  assert(!intcmp(&buf, v_get(&thing, 1)));

  v_remove(&thing, 0);
  assert(!intcmp(&buf, v_get(&thing, 0)));
  assert(0 == v_find(&thing, intcmp, &buf));
  buf = 0;
  assert(-1 == v_find(&thing, intcmp, &buf));

  v_remove(&thing, 0);
  assert(-1 == v_find(&thing, intcmp, &buf));
  buf = 0;
  assert(-1 == v_find(&thing, intcmp, &buf));

  v_compact(&thing);

  v_add(&thing, &buf);
  assert(0 == v_find(&thing, intcmp, &buf));
  assert(!intcmp(&buf, v_get(&thing, 0)));

  v_deinit(&thing);

  return 0;
}
