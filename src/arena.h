/*
 * Copyright (c) 2026 ClownEZZ
 * Licensed under the zlib License (see the LICENSE file)
 */

#ifndef __ARENA_H__
#define __ARENA_H__

#include <stddef.h>

#define ALIGMENT sizeof(void *)
#define ALIGN(x) ((x) + ALIGMENT - 1) & ~(ALIGMENT - 1)

typedef struct arena_t {
  void *data;
  size_t offset;
  size_t size;
  struct arena_t *next;
} arena_t;

arena_t *arena_create(size_t size);
void arena_term(arena_t *arena);
void *arena_alloc(arena_t *arena, size_t size);
void arena_reset(arena_t *arena);

#endif
