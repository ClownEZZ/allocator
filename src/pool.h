/*
 * Copyright (c) 2026 ClownEZZ
 * Licensed under the zlib License (see the LICENSE file)
 */

#ifndef __POOL_H__
#define __POOL_H__

#include "arena.h"

typedef struct pool_t {
  arena_t *arena;
  size_t   obj_size;
  void    *free_list;
} pool_t;

pool_t *pool_create(size_t obj_size, size_t count);
void    pool_term(pool_t *pool);
void   *pool_alloc(pool_t *pool);
void    pool_free(pool_t *pool, void *ptr);

#endif
