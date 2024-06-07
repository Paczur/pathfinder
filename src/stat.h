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

stats_t *stat(uchar *node_is, uchar nodesl, const char *str);

#endif
