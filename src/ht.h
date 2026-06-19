/*
 * Copyright (c) 2026 ClownEZZ
 * Licensed under the zlib License (see the LICENSE file)
 */

#ifndef __HT_H__
#define __HT_H__

#include "arena.h"

typedef struct {
  char *key;
  int   val;
  int   used;
} __entry_t;

typedef struct {
  __entry_t *entry;
  size_t     capacity;
  size_t     count;
  arena_t   *arena;
} ht_t;

ht_t *ht_create(size_t capacity);
void  ht_term(ht_t *ht);
void  ht_set(ht_t *ht, const char *key, int val);
int  *ht_get(ht_t *ht, const char *key);
void  ht_delete(ht_t *ht, const char *key);


#endif
