/*
 * Copyright (c) 2026 ClownEZZ
 * Licensed under the zlib License (see the LICENSE file)
 */

#include <stdlib.h>
#include <string.h>
#include "ht.h"

#define YES 1
#define NO  0

ht_t *ht_create(size_t cap)
{
  ht_t *ht;
  size_t i;

  if (!(cap && !(cap & (cap - 1))))
    return NULL;

  ht = malloc(sizeof(ht_t));
  if (ht == NULL)
    return NULL;

  ht->entry = malloc(sizeof(__entry_t) * cap);
  ht->arena = arena_create(cap * 16);
  if (ht->entry == NULL || ht->arena == NULL) {
    free(ht->entry);
    free(ht->arena);
    free(ht);
    return NULL;
  }

  for (i = 0; i < cap; i++)
    ht->entry[i].used = NO;
  ht->capacity = cap;
  ht->count = 0;

  return ht;
}

void ht_term(ht_t *ht) {
  if (ht == NULL) return;
  free(ht->entry);
  arena_term(ht->arena);
  free(ht);
}

static size_t gethash(const char *s);
static size_t search(ht_t *ht, const char *key);
static void   rehash(ht_t *ht0, ht_t *ht1);

void ht_set(ht_t *ht, const char *key, int val)
{
  size_t i;

  if (ht == NULL || !(ht->capacity && !(ht->capacity & (ht->capacity - 1))))
    return;

  if (ht->count * 100 / ht->capacity > 70) {
    __entry_t *tmp;
    size_t i;
    tmp = malloc(sizeof(__entry_t) * ht->capacity * 2);
    if (tmp == NULL)
      return;
    for (i = 0; i < ht->capacity * 2; i++)
      tmp[i].used = NO;
    rehash(ht, &(ht_t){tmp, ht->capacity * 2, 0, NULL});
    free(ht->entry);
    ht->entry = tmp;
    ht->capacity *= 2;
  }

  i = search(ht, key);
  if (!ht->entry[i].used) {
    ht->entry[i].key = arena_alloc(ht->arena, strlen(key) + 1);
    strcpy(ht->entry[i].key, key);
    ht->entry[i].used = YES;
    ht->count++;
  }
  ht->entry[i].val = val;
}

int *ht_get(ht_t *ht, const char *key)
{
  size_t i;

  if (ht == NULL || !(ht->capacity && !(ht->capacity & (ht->capacity - 1))))
    return NULL;

  i = search(ht, key);
  if (ht->entry[i].used)
    return &ht->entry[i].val;
  return NULL;
}

void ht_delete(ht_t *ht, const char *key)
{
  size_t i;

  if (ht == NULL || !(ht->capacity && !(ht->capacity & (ht->capacity - 1))))
    return;

  i = search(ht, key);
  if (!ht->entry[i].used)
    return;
  ht->entry[i].used = NO;
  ht->count--;
  rehash(ht, ht);
}

static void rehash(ht_t *ht0, ht_t *ht1)
{
  size_t i, j;
  for (i = 0; i < ht0->capacity; i++) {
    if (!ht0->entry[i].used)
      continue;

    j = gethash(ht0->entry[i].key) & (ht1->capacity - 1);

    while (ht1->entry[j].used)
      j = (j + 1) & (ht1->capacity - 1);

    ht1->entry[j].key  = ht0->entry[i].key;
    ht1->entry[j].val  = ht0->entry[i].val;
    ht1->entry[j].used = YES;
    ht0->entry[i].used = NO;
  }
}

/* FNV-1a */
static size_t search(ht_t *ht, const char *key)
{
  size_t i;

  i = gethash(key) & (ht->capacity - 1);
  while (ht->entry[i].used) {
    if (!strcmp(ht->entry[i].key, key))
      return i;
    i = (i + 1) & (ht->capacity - 1);
  }

  return i;
}

static size_t gethash(const char *s)
{
  size_t res;

  res = 14695981039346656037ULL;
  while (*s) {
    res ^= (unsigned char)*s++;
    res *= 1099511628211ULL;
  }

  return res;
}
