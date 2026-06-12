/*
 * Copyright (c) 2026 ClownEZZ
 * Licensed under the zlib License (see the LICENSE file)
 */

#include <stdlib.h>
#include "arena.h"

arena_t *arena_create(size_t size)
{
  arena_t *arena;

  arena = (arena_t *) malloc(sizeof(arena_t));
  if (arena != NULL && size != 0) {
    arena->data = malloc(size);
    arena->offset = 0;
    arena->size = size;
  }

  return arena;
}

void arena_term(arena_t *arena) {
  free(arena->data);
  free(arena);
}

void *arena_alloc(arena_t *arena, size_t size)
{
#define ALIGNMENT 8
  size_t tmp;
  tmp = (arena->offset + ALIGNMENT - 1) & ~(ALIGNMENT - 1);
  if (tmp + size <= arena->size && size != 0) {
    arena->offset = tmp + size;
    return (char*) arena->data + tmp;
  }
  return NULL;
#undef ALIGNMENT
}

void arena_reset(arena_t *arena) {
  arena->offset = 0;
}
