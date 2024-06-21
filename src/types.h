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

typedef struct resv_t {
  char *path;
  uint score;
#ifndef NDEBUG
  stats_t stats;
#endif
} resv_t;
void resv_print(const resv_t *val, uint count);

typedef struct resn_t {
  char *path;
  uint score;
#ifndef NDEBUG
  stats_t stats;
#endif
  struct resn_t *next;
} resn_t;
void resn_print(const resn_t *node, uint count);

typedef struct resl_t {
  resn_t *head;
  resn_t *tail;
} resl_t;

void resl_add(resl_t *list, resn_t *node);
void resl_print(const resl_t *list, uint count);

#endif
