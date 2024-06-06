#ifndef H_STAT
#define H_STAT

#include "types.h"

typedef struct stats_t {
  uchar depth;
  uchar *dirname_start;
} stats_t;

stats_t *stat(uchar *node_is, uchar nodesl, const char *str);

#endif
