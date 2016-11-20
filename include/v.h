#pragma once

typedef struct {
  uint8_t _slice_size;
  uint8_t _count;
  uint8_t _capacity;
  void *_buf;
} V;

extern void v_init(V *v, uint8_t slice_size);
extern void v_deinit(V *v);

inline int v_count(V *v) {
  return v->_count;
}

extern int v_add(V *v, void *slice);
extern void *v_get(V *v, uint8_t i);
extern void v_remove(V *v, uint8_t i);
extern void v_compact(V *v);

typedef int (*v_cmp_t)(void *a, void *b);

extern int v_find(V *v, v_cmp_t cmp, void *thing);
