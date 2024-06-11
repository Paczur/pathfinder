#ifndef H_UTIL
#define H_UTIL

#include <stddef.h>
#include <stdbool.h>

typedef unsigned char uchar;
typedef unsigned int uint;

typedef struct stats_t {
  uchar depth;
  uchar count;
  uchar *dirname_start;
  uchar *dirname_end;
  uchar *word_start;
  uchar *word_end;
  uchar *bad_case;
} stats_t;
#define STAT_INIT(size)                 \
  {                                     \
    .dirname_start = &(uchar[size]){0}, \
    .dirname_end = &(uchar[size]){0},   \
    .word_start = &(uchar[size]){0},    \
    .word_end = &(uchar[size]){0},      \
    .bad_case = &(uchar[size]){0},      \
  }
void stats_alloc(stats_t **stats, uchar node_count);
void stats_free(stats_t **stats);

#endif
