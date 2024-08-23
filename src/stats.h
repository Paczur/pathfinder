#ifndef H_STAT
#define H_STAT

#include "types.h"

typedef struct stats_t {
  uint depth;
  uint *dirname_start;
  uint *dirname_end;
  uint *word_start;
  uint *word_end;
  uint *up_case;
  uint *low_case;
  bool *dotfile;
} stats_t;
#define STATS_INIT(size)               \
  {                                    \
    .dirname_start = &(uint[size]){0}, \
    .dirname_end = &(uint[size]){0},   \
    .word_start = &(uint[size]){0},    \
    .word_end = &(uint[size]){0},      \
    .low_case = &(uint[size]){0},      \
    .up_case = &(uint[size]){0},       \
    .dotfile = &(bool[size]){0},       \
  }
void stats(stats_t *stats, uint *node_is, uint nodesl,
           const char *restrict const *expr, const char *restrict str);
void stats_alloc(stats_t *stats, uint node_count);
void stats_free(stats_t *stats);
void stats_print(const stats_t *stats, uint count);

#endif
