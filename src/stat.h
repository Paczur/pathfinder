#ifndef H_STAT
#define H_STAT

#include "types.h"

typedef struct stats_t {
  uchar depth;
  uchar count;
  uchar *dirname_start;
  uchar *dirname_end;
  uchar *word_start;
  uchar *word_end;
  uchar *good_case;
} stats_t;

void stat(stats_t *stats, uchar *node_is, uchar nodesl, const char *expr,
          const char *str);

void stats_alloc(stats_t **stats, uchar node_count);

void stats_free(stats_t **stats);

#define STAT_INIT(size)                 \
  {                                     \
    .dirname_start = &(uchar[size]){0}, \
    .dirname_end = &(uchar[size]){0},   \
    .word_start = &(uchar[size]){0},    \
    .word_end = &(uchar[size]){0},      \
    .good_case = &(uchar[size]){0},     \
  }
#endif
