/*
 * Copyright (c) 2026 ClownEZZ
 * Licensed under the zlib License (see the LICENSE file)
 */

#include <stdlib.h>
#include "pool.h"

pool_t *pool_create(size_t obj_size, size_t count)
{
  size_t i, block_size;
  pool_t *pool;
  char *p;

  if (obj_size == 0 || count == 0)
    return NULL;

  pool = (pool_t *) malloc(sizeof(pool_t));
  if (pool == NULL)
    return NULL;

  block_size = obj_size;
  if (block_size < sizeof(void *))
    block_size = sizeof(void *);
  block_size = ALIGN(block_size);

  pool->arena = arena_create(block_size * count);
  if (pool->arena == NULL) {
    free(pool);
    return NULL;
  }

  pool->obj_size = obj_size;
  pool->free_list = NULL;

  p = (char *) pool->arena->data;
  for (i = 0; i < count; i++) {
    *(void **) p = pool->free_list;
    pool->free_list = p;
    p += block_size;
  }
  pool->arena->offset = block_size * count;

  return pool;
}

void pool_term(pool_t *pool)
{
  if (pool == NULL) return;
  arena_term(pool->arena);
  free(pool);
}

void *pool_alloc(pool_t *pool)
{
  void *block;

  if (pool == NULL)
    return NULL;

  if (pool->free_list == NULL) {
#define NEW_ARENA_SIZE 64
    size_t i, block_size;
    arena_t *arena;
    char *p;

    arena = pool->arena;
    while (arena->next != NULL)
      arena = arena->next;

    block_size = pool->obj_size;
    if (block_size < sizeof(void *))
      block_size = sizeof(void *);

    arena->next = arena_create(block_size * NEW_ARENA_SIZE);
    arena = arena->next;
    if (arena == NULL)
      return NULL;

    p = (char *) arena->data;
    for (i = 0; i < NEW_ARENA_SIZE; i++) {
      *(void **) p = pool->free_list;
      pool->free_list = p;
      p += block_size;
    }
    arena->offset = block_size * NEW_ARENA_SIZE;
#undef NEW_ARENA_SIZE
  }

  block = pool->free_list;
  pool->free_list = *(void **) block;

  return block;
}

void pool_free(pool_t *pool, void *p)
{
  if (pool == NULL || p == NULL) return;
  *(void **) p = pool->free_list;
  pool->free_list = p;
}
