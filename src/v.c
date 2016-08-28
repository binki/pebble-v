#include <pebble.h>
#include "v.h"

void v_init(V *v, uint8_t slice_size)
{
  v->slice_size = slice_size;
  v->count = 0;
  v->capacity = 0;
  v->buf = NULL;
}

void v_deinit(V *v)
{
  free(v->buf);
}

static int v_expand(V *v, uint_t count)
{
  void *new_buf = realloc(v->buf, count * v->slice_size);
  if (!new_buf) {
    return 1;
  }

  v->buf = new_buf;
  v->count = count;
  return 0;
}

int v_add(V *v, void *slice) {
  if (v->count == v->capacity) {
    // Expand if possible.
    if (v_expand(v, v->count + 4)) {
      if (v_expand(v, v->count + 1)) {
        return 1;
      }
    }
    memcpy(v->buf + v->count*v->slice_size, slice, v->slice_size);
    return 0;
  }
}

void *v_get(V *v, uint8_t i) {
  return &v->buf[i * v->slice_size];
}

void v_remove(V *v, uint8_t i) {
  v->count--;
  if (i == v->count) {
    return;
  }
  memmove(v->buf + i * v->slice_size, v->buf + (i + 1) * v->slice_size, (v->count - i) * v->slice_size);
}

void v_compact(V *v)
{
  if (v->count == v->capacity) {
    return;
  }

  // See if we can allocate a replacement buffer
  v_expand(v, v->count);
}

int v_find(V *v, v_cmp_t cmp, void *thing) {
  for (int i = 0; i < v->count; i++) {
    int result = 0;
    if (cmp) {
      result = cmp(v_get(v, i), thing);
    } else {
      result = memcmp(v_get(v, i), thing, v->slice_size);
    }
    if (!result) {
      return result;
    }
  }
  return -1;
}
