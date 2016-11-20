/*!
 * Copyright (C) 2016 Nathan Phillip Brink
 * Distributed under the terms of the MIT license, see LICENSE.md.
 */
#ifndef PEBBLE_V_TEST
#include <pebble.h>
#include "v.h"
#endif

extern inline int v_count(V *v);

void v_init(V *v, uint8_t slice_size)
{
  v->_slice_size = slice_size;
  v->_count = 0;
  v->_capacity = 0;
  v->_buf = NULL;
}

void v_deinit(V *v)
{
  free(v->_buf);
}

static int v_expand(V *v, uint8_t capacity)
{
  void *new_buf = NULL;
  if (capacity) {
    new_buf = realloc(v->_buf, capacity * v->_slice_size);
    if (!new_buf) {
      return 1;
    }
  } else {
    // If capacity is 0, set to NULL. realloc() is allowed to do weird
    // things in this case and easier to just avoid calling it with
    // size=0.
    free(v->_buf);
  }

  v->_buf = new_buf;
  v->_capacity = capacity;
  return 0;
}

int v_add(V *v, void *slice) {
  if (v->_count == v->_capacity) {
    // Expand if possible.
    if (v_expand(v, v->_count + 4)) {
      if (v_expand(v, v->_count + 1)) {
        return 1;
      }
    }
  }
  memcpy(&((uint8_t *)v->_buf)[v->_count*v->_slice_size], slice, v->_slice_size);
  v->_count++;
  return 0;
}

void *v_get(V *v, uint8_t i) {
  return &((uint8_t*)v->_buf)[i * v->_slice_size];
}

void v_remove(V *v, uint8_t i) {
  v->_count--;
  if (i == v->_count) {
    return;
  }
  memmove(&((uint8_t *)v->_buf)[i * v->_slice_size], &((uint8_t *)v->_buf)[(i + 1) * v->_slice_size], (v->_count - i) * v->_slice_size);
}

void v_compact(V *v)
{
  if (v->_count == v->_capacity) {
    return;
  }

  // See if we can allocate a replacement buffer
  v_expand(v, v->_count);
}

int v_find(V *v, v_cmp_t cmp, void *thing) {
  for (int i = 0; i < v->_count; i++) {
    int result = 0;
    if (cmp) {
      result = cmp(v_get(v, i), thing);
    } else {
      result = memcmp(v_get(v, i), thing, v->_slice_size);
    }
    if (!result) {
      return i;
    }
  }
  return -1;
}

static int v_testsuite_intcmp(void *a, void *b) {
  return memcmp(a, b, sizeof(int));
}
