#ifndef H_STAT
#define H_STAT

#include "types.h"

void stat(stats_t *stats, uint *node_is, uint nodesl, const char *const *expr,
          const char *str);

#endif
