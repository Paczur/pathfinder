#ifndef H_STAT
#define H_STAT

#include "types.h"

typedef struct stats_t {
  uchar depth;
} stats_t;

stats_t *stat(const char *expr, const char *str);

#endif
