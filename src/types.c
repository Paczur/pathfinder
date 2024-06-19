#include "types.h"
#include <stdlib.h>

void stats_alloc(stats_t *stats, uchar node_count) {
  stats->depth = 0;
  stats->count = 0;
  stats->dirname_start = malloc(sizeof(node_count));
  stats->dirname_end = malloc(sizeof(node_count));
  stats->word_start = malloc(sizeof(node_count));
  stats->word_end = malloc(sizeof(node_count));
  stats->bad_case = malloc(sizeof(node_count));
}

void stats_free(stats_t *stats) {
  free(stats->dirname_start);
  free(stats->dirname_end);
  free(stats->word_start);
  free(stats->word_end);
  free(stats->bad_case);
}
