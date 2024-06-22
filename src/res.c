#include "res.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void resv_print(const resv_t *val, uint count) {
  (void)count;
  printf("{path: \"%s\", score: %u", val->path, val->score);
#ifndef NDEBUG
  printf(", stats: ");
  stats_print(&val->stats, count);
#endif
  printf("}");
}

void resn_free(resn_t *node) {
  free(node->path);
#ifndef NDEBUG
  stats_free(&node->stats);
#endif
  free(node);
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

void resa_add(resa_t *arr, resv_t *val) {
  for(size_t i = 0; i < arr->limit; i++) {
    if(arr->arr[i].score < val->score) {
      arr->size = (arr->size == arr->limit) ? arr->limit : arr->size + 1;
      memmove(arr->arr + i + 1, arr->arr + i,
              (arr->size - i - 1) * sizeof(resv_t));
      arr->arr[i] = *val;
      break;
    }
  }
}

void resa_print(const resa_t *arr, uint count) {
  printf("[");
  for(size_t i = 0; i < arr->size - 1; i++) {
    resv_print(arr->arr + i, count);
    puts(",");
  }
  resv_print(arr->arr + arr->size - 1, count);
  printf("]");
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
