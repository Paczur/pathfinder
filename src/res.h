#ifndef H_RES
#define H_RES

#include "types.h"

typedef struct resv_t {
  char *path;
  uint score;
#ifndef NDEBUG
  stats_t stats;
#endif
} resv_t;
void resv_print(const resv_t *val, uint count);

typedef struct resn_t {
  struct resn_t *next;
  char *path;
  uint score;
#ifndef NDEBUG
  stats_t stats;
#endif
} resn_t;
void resn_free(resn_t *node);
void resn_print(const resn_t *node, uint count);

typedef struct resa_t {
  uint limit;
  uint size;
  resv_t *arr;
} resa_t;
void resa_add(resa_t *arr, resv_t *val);
void resa_print(const resa_t *arr, uint count);

typedef struct resl_t {
  resn_t *head;
  resn_t *tail;
} resl_t;
void resl_add(resl_t *list, resn_t *node);
void resl_print(const resl_t *list, uint count);

#endif
