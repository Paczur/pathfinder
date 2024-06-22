#include "types.h"
#include <stdlib.h>
#include <stdio.h>

void stats_alloc(stats_t *stats, uint node_count) {
  stats->depth = 0;
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

void stats_print(const stats_t *stats, uint count) {
  printf("{depth: %u, dirname_start: [", stats->depth);
  for(size_t i = 0; i < count - 1; i++) {
    printf("%u, ", stats->dirname_start[i]);
  }
  printf("%u]", stats->dirname_start[count - 1]);
  printf(", dirname_end: [");
  for(size_t i = 0; i < count - 1; i++) {
    printf("%u, ", stats->dirname_end[i]);
  }
  printf("%u]", stats->dirname_end[count - 1]);
  printf(", word_start: [");
  for(size_t i = 0; i < count - 1; i++) {
    printf("%u, ", stats->word_start[i]);
  }
  printf("%u]", stats->word_start[count - 1]);
  printf(", word_end: [");
  for(size_t i = 0; i < count - 1; i++) {
    printf("%u, ", stats->word_end[i]);
  }
  printf("%u]", stats->word_end[count - 1]);
  printf(", bad_case: [");
  for(size_t i = 0; i < count - 1; i++) {
    printf("%u, ", stats->bad_case[i]);
  }
  printf("%u]", stats->bad_case[count - 1]);
  printf("}");
}
