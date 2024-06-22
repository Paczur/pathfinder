#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "match.h"
#include "stat.h"
#include "score.h"
#include "res.h"

#define HELP_MSG                                               \
  "Usage: pf [OPTION]... EXPR...\n"                            \
  "Find path(s) best matching EXPR using substring matches.\n" \
  "\n"                                                         \
  "General options:\n"                                         \
  "-h, --help          Show help\n"                            \
  "-m, --max-matches   Number of matches to print (default 30)"

uint *ranges;
bool limited = true;
bool interactive = false;
resl_t list;
resa_t arr = {.size = 0, .limit = 1};

static void cleanup(void) {
  if(limited) {
    resa_free(&arr);
  } else {
    resl_free(&list);
  }
}

static uint node_count(const char *const *expr, uint len) {
  uint res = len;
  for(size_t i = 0; i < len; i++) {
    for(size_t j = 0; expr[i][j]; j++) {
      if(expr[i][j] == '/') res++;
    }
  }
  return res;
}

static uint handleinf(const char *str, const char *const *expr, uint len,
                      uint count) {
  resn_t *new;
#ifdef NDEBUG
  stats_t stats;
#endif
  if(matches(ranges, str, expr, len)) {
    new = malloc(sizeof(resn_t));
    new->path = malloc(strlen(str) + 1);
    strcpy(new->path, str);
#ifndef NDEBUG
    stats_alloc(&new->stats, len);
    stat(&new->stats, ranges, len * 2, expr, str);
    new->score = score(&new->stats, count);
#else
    stats_alloc(&stats, len);
    stat(&stats, ranges, len * 2, expr, str);
    new->score = score(&stats, count);
    stats_free(&stats);
#endif
    resl_add(&list, new);
    return new->score;
  }
  return 0;
}

static uint handle(const char *str, const char *const *expr, uint len,
                   uint count) {
  resv_t new;
#ifdef NDEBUG
  stats_t stats;
#endif
  if(matches(ranges, str, expr, len)) {
    new.path = malloc(strlen(str) + 1);
    strcpy(new.path, str);
#ifndef NDEBUG
    stats_alloc(&new.stats, len);
    stat(&new.stats, ranges, len * 2, expr, str);
    new.score = score(&new.stats, count);
#else
    stats_alloc(&stats, len);
    stat(&stats, ranges, len * 2, expr, str);
    new.score = score(&stats, count);
    stats_free(&stats);
#endif
    if(!resa_add(&arr, &new)) {
      free(new.path);
#ifndef NDEBUG
      stats_free(&new.stats);
#endif
    } else {
      return new.score;
    }
  }
  return 0;
}

static void rec_paths(const char *const *expr, uint len, uint count,
                      uint (*f)(const char *, const char *const *, uint, uint),
                      char *path) {
  struct dirent *de;
  DIR *dr = opendir(path);
  size_t null = strlen(path);
  if(!dr) {
    printf("Couldn't open directory: %s\n", path);
    return;
  }
  while((de = readdir(dr))) {
    if(!strcmp(de->d_name, "..") || !strcmp(de->d_name, ".")) continue;
    if(de->d_type == DT_DIR) {
      sprintf(path + null, "/%s", de->d_name);
      if(arr.limit == 1) {
        if(SCORE_BASE == f(path + 2, expr, len, count)) break;
      } else {
        f(path + 2, expr, len, count);
      }
      rec_paths(expr, len, count, f, path);
      path[null] = 0;
    }
  }
  closedir(dr);
}

static void find_paths(const char *const *expr, uint len, uint count) {
  uint (*f)(const char *, const char *const *, uint, uint) =
    (limited) ? handle : handleinf;
  char path[512] = ".";
  rec_paths(expr, len, count, f, path);
}

int main(int argc, const char *const argv[]) {
  uint nc;
  long long t;
  uint off = 1;
  if(argc < 2) goto error;
  for(; off < (uint)argc; off++) {
    if(argv[off][0] != '-') break;
    if(!strcmp(argv[off], "-h") || !strcmp(argv[off], "--help")) {
      puts(HELP_MSG);
      goto cleanup;
    } else if(!strcmp(argv[off], "-m") || !strcmp(argv[off], "--max-matches")) {
      if(off + 1 >= (uint)argc || !sscanf(argv[off + 1], "%lld", &t)) {
        goto error;
      }
      if(t < 1) {
        limited = false;
      } else {
        arr.limit = (uint)t;
      }
      off++;
    }
  }
  nc = node_count(argv + off, argc - off);

  ranges = malloc((argc - 1) * 2 * sizeof(uint));
  if(limited) arr.arr = calloc(arr.limit, sizeof(resv_t));
  find_paths(argv + off, argc - off, nc);

  if(limited) {
    resa_path_print(&arr);
  } else {
    resl_path_print(&list);
  }

cleanup:
  cleanup();
  free(ranges);
  return 0;

error:
  puts(HELP_MSG);
  cleanup();
  free(ranges);
  return 1;
}
