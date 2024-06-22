#ifndef H_UTIL
#define H_UTIL

#include <stddef.h>
#include <stdbool.h>

typedef unsigned char uchar;
typedef unsigned int uint;

typedef struct stats_t {
  uint depth;
  uint *dirname_start;
  uint *dirname_end;
  uint *word_start;
  uint *word_end;
  uint *bad_case;
} stats_t;
#define STAT_INIT(size)                \
  {                                    \
    .dirname_start = &(uint[size]){0}, \
    .dirname_end = &(uint[size]){0},   \
    .word_start = &(uint[size]){0},    \
    .word_end = &(uint[size]){0},      \
    .bad_case = &(uint[size]){0},      \
  }
void stats_alloc(stats_t *stats, uint node_count);
void stats_free(stats_t *stats);
void stats_print(const stats_t *stats, uint count);

#endif
