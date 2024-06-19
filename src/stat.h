#ifndef H_STAT
#define H_STAT

#include "types.h"

void stat(stats_t *stats, uchar *node_is, uchar nodesl, const char *const *expr,
          const char *str);

#endif
