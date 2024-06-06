#include "stat.h"
#include <assert.h>
#include <stdlib.h>

static uchar depth(const char *str) {
  assert(str[0]);
  uchar counter = 1;
  if(!str[1]) return str[0] != '/';
  for(size_t i = 1; str[i + 1]; i++) {
    if(str[i] == '/') counter++;
  }
  return counter;
}

stats_t *stat(const char *expr, const char *str) {
  stats_t *stats = malloc(sizeof(stats_t));
  stats->depth = depth(str);
  return stats;
}
