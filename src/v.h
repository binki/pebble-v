#pragma once

#include <pebble.h>

typedef struct {
  uint8_t slice_size;
  uint8_t count;
  uint8_t capacity;
  void *buf;
} V;

extern void v_init(V *v, uint8_t slice_size);
extern void v_deinit(V *v);

extern int v_add(V *v, void *slice);
extern void *v_get(V *v, uint8_t i);
extern void v_remove(V *v, uint8_t i);
extern void v_compact(V *v);

typedef int (*v_cmp_t)(void *a, void *b);

extern int v_find(V *v, v_cmp_t cmp, void *thing);

extern void v_testsuite();
