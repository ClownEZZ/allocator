/*
 * Copyright (c) 2026 ClownEZZ
 * Licensed under the zlib License (see the LICENSE file)
 */

#ifndef __ARENA_H__
#define __ARENA_H__

typedef struct {
  void *data;
  size_t offset;
  size_t size;
} arena_t;

arena_t *arena_create(size_t size);
void arena_term(arena_t *arena);
void *arena_alloc(arena_t *arena, size_t size);
void arena_reset(arena_t *arena);

#endif
