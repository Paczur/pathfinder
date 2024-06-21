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

void resn_print(const resn_t *node, uint count) {
  (void)count;
  printf("{path: \"%s\", score: %u", node->path, node->score);
#ifndef NDEBUG
  printf(", stats: ");
  stats_print(&node->stats, count);
#endif
  printf(", next: %p}", node->next);
}

void resl_add(resl_t *list, resn_t *node) {
  resn_t *n = list->head;
  if(!list->head) {
    list->head = node;
    list->tail = node;
    node->next = NULL;
    return;
  }
  if(list->head->score < node->score) {
    node->next = list->head;
    list->head = node;
    return;
  }
  while(n != list->tail) {
    if(node->score > n->next->score) {
      node->next = n->next;
      n->next = node;
      return;
    }
    n = n->next;
  }
  list->tail->next = node;
  list->tail = node;
  node->next = NULL;
}

void resl_print(const resl_t *list, uint count) {
  resn_t *n = list->head;
  printf("{head: %p, tail: %p,\n", list->head, list->tail);
  while(n != list->tail) {
    resn_print(n, count);
    puts(", ");
    n = n->next;
  }
  resn_print(n, count);
  puts("}");
}
