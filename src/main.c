#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "match.h"
#include "stat.h"
#include "score.h"

uint *ranges;
resl_t list;

uint node_count(const char *const *expr, uint len) {
  uint res = len;
  for(size_t i = 0; i < len; i++) {
    for(size_t j = 0; expr[i][j]; j++) {
      if(expr[i][j] == '/') res++;
    }
  }
  return res;
}

void handle(const char *str, const char *const *expr, uint len, uint count) {
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
  }
}

int main(int argc, const char *const argv[]) {
  DIR *dr;
  uint nc;
  struct dirent *de;
  if(argc < 2) {
    puts("HELP MSG HERE");
    return 1;
  }
  nc = node_count(argv + 1, argc - 1);
  dr = opendir(".");
  if(!dr) {
    puts("Couldn't open directory");
    return 1;
  }

  ranges = malloc((argc - 1) * 2 * sizeof(uint));

  while((de = readdir(dr))) {
    if(!strcmp(de->d_name, "..") || !strcmp(de->d_name, ".")) continue;
    if(de->d_type == DT_DIR) {
      handle(de->d_name, argv + 1, argc - 1, nc);
    }
  }
  closedir(dr);
  resl_print(&list, nc);
  return 0;
}
