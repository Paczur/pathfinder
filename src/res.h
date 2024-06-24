#ifndef H_RES
#define H_RES

#include "types.h"
#include "stats.h"

typedef struct resv_t {
  char *path;
  uint score;
#ifndef NDEBUG
  stats_t stats;
#endif
} resv_t;
void resv_alloc(resv_t *val, size_t len, size_t count);
void resv_free(resv_t *val);
void resv_print(const resv_t *val, uint count);

typedef struct resn_t {
  char *path;
  uint score;
#ifndef NDEBUG
  stats_t stats;
#endif
  struct resn_t *next;
} resn_t;
void resn_alloc(resn_t *node, size_t len, size_t count);
void resn_free(resn_t *node);
void resn_print(const resn_t *node, uint count);

#define RESA_INIT(lim) {.arr = &(resv_t[lim]){{0}}, .limit = lim}
typedef struct resa_t {
  uint limit;
  uint size;
  resv_t *arr;
} resa_t;
bool resa_add(resa_t *arr, resv_t *val);
void resa_alloc(resa_t *arr);
void resa_free(resa_t *arr);
void resa_print(const resa_t *arr, uint count);
void resa_path_print(const resa_t *arr);

typedef struct resl_t {
  resn_t *head;
  resn_t *tail;
} resl_t;
void resl_add(resl_t *list, resn_t *node);
void resl_free(resl_t *list);
void resl_print(const resl_t *list, uint count);
void resl_path_print(const resl_t *list);

#endif
