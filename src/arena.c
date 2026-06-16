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
    if (arena->data == NULL) {
      free(arena);
      return NULL;
    }
    arena->offset = 0;
    arena->size = size;
    arena->next = NULL;
    return arena;
  }

  return NULL;
}

void arena_term(arena_t *arena) {
  if (arena->next != NULL)
    arena_term(arena->next);
  free(arena->data);
  free(arena);
}

void *arena_alloc(arena_t *arena, size_t size)
{
  size_t tmp;

  if (arena == NULL || size == 0)
    return NULL;

  tmp = ALIGN(arena->offset);
  if (tmp + size <= arena->size) {
    arena->offset = tmp + size;
    return (char*) arena->data + tmp;
  }
  else {
    if (arena->next == NULL)
      arena->next = arena_create(size + 1000);
    return arena_alloc(arena->next, size);
  }

  return NULL;
}

void arena_reset(arena_t *arena) {
  if (arena->next != NULL)
    arena_reset(arena->next);
  arena->offset = 0;
}
