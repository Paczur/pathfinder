#include "res.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void resv_alloc(resv_t *val, size_t len, size_t count) {
#ifndef NDEBUG
  stats_alloc(&val->stats, count);
#else
  (void)count;
#endif
  val->path = malloc(len);
}

void resv_free(resv_t *val) {
  free(val->path);
#ifndef NDEBUG
  stats_free(&val->stats);
#endif
}

void resv_print(const resv_t *val, uint count) {
  (void)count;
  printf("{path: \"%s\", score: %u", val->path, val->score);
#ifndef NDEBUG
  printf(", stats: ");
  stats_print(&val->stats, count);
#endif
  printf("}");
}

void resn_alloc(resn_t *node, size_t len, size_t count) {
  node->path = malloc(len);
#ifndef NDEBUG
  stats_alloc(&node->stats, count);
#endif
}

void resn_free(resn_t *node) {
  free(node->path);
#ifndef NDEBUG
  stats_free(&node->stats);
#endif
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

bool resa_add(resa_t *arr, resv_t *val) {
  for(size_t i = 0; i < arr->size; i++) {
    if(arr->arr[i].score < val->score) {
      if(arr->size == arr->limit) {
        free(arr->arr[arr->size - 1].path);
#ifndef NDEBUG
        stats_free(&arr->arr[arr->size - 1].stats);
#endif
      }
      arr->size = (arr->size == arr->limit) ? arr->limit : arr->size + 1;
      memmove(arr->arr + i + 1, arr->arr + i,
              (arr->size - i - 1) * sizeof(resv_t));
      arr->arr[i] = *val;
      return true;
    }
  }
  if(arr->size < arr->limit) {
    arr->arr[arr->size] = *val;
    arr->size = (arr->size == arr->limit) ? arr->limit : arr->size + 1;
    return true;
  }
  return false;
}

void resa_alloc(resa_t *arr) {
  arr->arr = malloc(arr->limit * sizeof(arr->arr[0]));
}

void resa_free(resa_t *arr) {
  for(size_t i = 0; i < arr->size; i++) {
    resv_free(arr->arr + i);
  }
  free(arr->arr);
}

void resa_print(const resa_t *arr, uint count) {
  printf("[");
  if(arr->size > 0) {
    for(size_t i = 0; i < arr->size - 1; i++) {
      resv_print(arr->arr + i, count);
      puts(",");
    }
    resv_print(arr->arr + arr->size - 1, count);
  }
  printf("]");
}

void resa_path_print(const resa_t *arr) {
  for(size_t i = 0; i < arr->size; i++) {
    printf("%s\n", arr->arr[i].path);
  }
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

void resl_free(resl_t *list) {
  resn_t *n = list->head;
  resn_t *t;
  while(n != list->tail) {
    t = n->next;
    resn_free(n);
    free(n);
    n = t;
  }
  resn_free(n);
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
  printf("}");
}

void resl_path_print(const resl_t *list) {
  resn_t *n = list->head;
  while(n != list->tail) {
    printf("%s\n", n->path);
    n = n->next;
  }
  printf("%s\n", n->path);
}
