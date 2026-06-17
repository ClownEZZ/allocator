/*
 * Copyright (c) 2026 ClownEZZ
 * Licensed under the zlib License (see the LICENSE file)
 */

#ifndef __SB_H__
#define __SB_H__

#include <stddef.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
  size_t count;
  size_t capacity;
} sb_header_t;

#define __sb_header(arr) ((sb_header_t*)((char*)(arr) - sizeof(sb_header_t)))
#define sb_count(arr) ((arr) ? __sb_header(arr)->count : 0)
#define sb_capacity(arr) ((arr) ? __sb_header(arr)->capacity : 0)

#define sb_push(arr, item) do {                                   \
  if ((arr) == NULL || sb_count(arr) == sb_capacity(arr)) {       \
    size_t new_cap;                                               \
    void *tmp;                                                    \
    new_cap = (arr) ? sb_capacity(arr) * 2 : 8;                   \
    tmp = malloc(sizeof(sb_header_t) + new_cap * sizeof(*(arr))); \
    if (tmp == NULL) break;                                       \
    ((sb_header_t*)tmp)->count = sb_count(arr);                   \
    ((sb_header_t*)tmp)->capacity = new_cap;                      \
    if (arr) {                                                    \
      memcpy((char*)tmp + sizeof(sb_header_t), arr, sb_count(arr) * sizeof(*(arr))); \
      free(__sb_header(arr));                                     \
    }                                                             \
    (arr) = (__typeof__(*(arr))*)((char*)tmp + sizeof(sb_header_t)); \
  }                                                               \
  (arr)[__sb_header(arr)->count++] = (item);                      \
} while(0);

#define sb_pop(arr) do { \
  if (sb_count(arr) > 0) __sb_header(arr)->count--; \
} while(0);

#define sb_free(arr) do { \
  if (arr) {              \
    free(__sb_header(arr)); \
    (arr) = NULL;         \
  }                       \
} while(0);

#endif
