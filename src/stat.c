#include "stat.h"
#include "match.h"
#include <assert.h>
#include <stdlib.h>
#include <stdio.h>

static uchar depth(const char *str) {
  assert(str);
  assert(str[0]);
  uchar counter = 1;
  if(!str[1]) return str[0] != '/';
  for(size_t i = 1; str[i + 1]; i++) {
    if(str[i] == '/') counter++;
  }
  return counter;
}

static uchar *dirname_start_distance(const uchar *node_is, uchar nodesl,
                                     const char *str) {
  assert(nodesl > 0);
  assert(node_is);
  assert(str);
  uchar *ret = malloc(nodesl * sizeof(uchar));
  uchar dist;
  uchar c;
  for(size_t i = 0; i < nodesl; i++) {
    dist = 0;
    c = node_is[i];
    for(; c >= dist && str[c - dist] != '/'; dist++) {
    }
    ret[i] = dist - 1;
  }
  return ret;
}

stats_t *stat(uchar *node_is, uchar nodesl, const char *str) {
  stats_t *stats = malloc(sizeof(stats_t));
  stats->depth = depth(str);
  stats->dirname_start = dirname_start_distance(node_is, nodesl, str);
  return stats;
}
