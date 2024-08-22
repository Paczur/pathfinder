#include "stats.h"
#include "match.h"
#include <assert.h>
#include <stdlib.h>
#include <stdio.h>

static uint depth(const char *str) {
  assert(str);
  assert(str[0]);
  uint counter = 1;
  if(!str[1]) return str[0] != '/';
  for(size_t i = 1; str[i + 1]; i++) {
    if(str[i] == '/') counter++;
  }
  return counter;
}

rda(1)
  rda(4) static void dirname_start_distance(const uint *ranges, uint rangesl,
                                            uint *ret, const char *str) {
  assert(rangesl > 0);
  assert(ranges);
  assert(str);
  assert(str[0]);
  assert(ret);
  uint dist;
  uint c;
  for(size_t i = 0; i < rangesl / 2; i++) {
    dist = 0;
    c = ranges[i * 2];
    for(; c >= dist && str[c - dist] != '/'; dist++) {
    }
    ret[i] = dist - 1;
  }
}

rda(1) rda(4) static void dirname_end_distance(const uint *ranges, uint rangesl,
                                               uint *ret, const char *str) {
  assert(rangesl > 0);
  assert(ranges);
  assert(str);
  assert(str[0]);
  assert(ret);
  uint dist;
  uint c;
  for(size_t i = 0; i < rangesl / 2; i++) {
    dist = 0;
    c = ranges[i * 2 + 1];
    for(; str[c + dist] && str[c + dist] != '/'; dist++) {
    }
    ret[i] = dist;
  }
}

rda(1) rda(4) static void word_start_distance(const uint *ranges, uint rangesl,
                                              uint *ret, const char *str) {
  assert(rangesl > 0);
  assert(ranges);
  assert(str);
  assert(str[0]);
  assert(ret);
  uint dist;
  uint c;
  for(size_t i = 0; i < rangesl / 2; i++) {
    dist = 0;
    c = ranges[i * 2];
    for(; c >= dist && ((str[c - dist] >= 'A' && str[c - dist] <= 'Z') ||
                        (str[c - dist] >= 'a' && str[c - dist] <= 'z'));
        dist++) {
    }
    ret[i] = dist - 1;
  }
}

rda(1) rda(4) static void word_end_distance(const uint *ranges, uint rangesl,
                                            uint *ret, const char *str) {
  assert(rangesl > 0);
  assert(ranges);
  assert(str);
  assert(str[0]);
  assert(ret);
  uint dist;
  uint c;
  for(size_t i = 0; i < rangesl / 2; i++) {
    dist = 0;
    c = ranges[i * 2 + 1];
    for(; str[c + dist] && ((str[c + dist] >= 'A' && str[c + dist] <= 'Z') ||
                            (str[c + dist] >= 'a' && str[c + dist] <= 'z'));
        dist++) {
    }
    ret[i] = dist;
  }
}

rda(1) rda(4)
  rda(5) static void up_case_count(const uint *ranges, uint rangesl, uint *ret,
                                   const char *str, const char *const *expr) {
  assert(rangesl > 0);
  assert(ranges);
  assert(ret);
  assert(str);
  assert(str[0]);
  assert(expr);
  assert(expr[0]);
  assert(expr[0][0]);
  size_t counter = 0;
  size_t ranges_i = 0;
  size_t index = 0;
  size_t slash = 0;
  for(size_t j = 0; ranges_i + 1 < rangesl; j++) {
    for(size_t i = 0; expr[j][i]; i++) {
      if(expr[j][i] == '/') {
        ret[index++] = counter;
        counter = 0;
        slash = i + 1;
        ranges_i += 2;
        continue;
      }
      if(expr[j][i] > str[ranges[ranges_i] + (slash ? i - slash : i)]) {
        counter++;
      }
    }
    ret[index++] = counter;
    counter = 0;
    ranges_i += 2;
    slash = 0;
  }
}

rda(1) rda(4)
  rda(5) static void low_case_count(const uint *ranges, uint rangesl, uint *ret,
                                    const char *str, const char *const *expr) {
  assert(rangesl > 0);
  assert(ranges);
  assert(ret);
  assert(str);
  assert(str[0]);
  assert(expr);
  assert(expr[0]);
  assert(expr[0][0]);
  size_t counter = 0;
  size_t ranges_i = 0;
  size_t index = 0;
  size_t slash = 0;
  for(size_t j = 0; ranges_i + 1 < rangesl; j++) {
    for(size_t i = 0; expr[j][i]; i++) {
      if(expr[j][i] == '/') {
        ret[index++] = counter;
        counter = 0;
        slash = i + 1;
        ranges_i += 2;
        continue;
      }
      if(expr[j][i] < str[ranges[ranges_i] + (slash ? i - slash : i)]) {
        counter++;
      }
    }
    ret[index++] = counter;
    counter = 0;
    ranges_i += 2;
    slash = 0;
  }
}

rda(1) rda(4) static void dotfile(const uint *ranges, uint rangesl, bool *ret,
                                  const char *str, uint *dirname_start) {
  ret[0] = str[ranges[0] - dirname_start[0]] == '.';
  for(size_t i = 1; i < rangesl / 2; i++) {
    ret[i] = str[ranges[i * 2] - dirname_start[i]] == '.';
  }
}

void stats_alloc(stats_t *stats, uint node_count) {
  stats->depth = 0;
  stats->dirname_start = malloc(node_count * sizeof(uint));
  stats->dirname_end = malloc(node_count * sizeof(uint));
  stats->word_start = malloc(node_count * sizeof(uint));
  stats->word_end = malloc(node_count * sizeof(uint));
  stats->up_case = malloc(node_count * sizeof(uint));
  stats->low_case = malloc(node_count * sizeof(uint));
  stats->dotfile = malloc(node_count * sizeof(bool));
}

void stats_free(stats_t *stats) {
  free(stats->dirname_start);
  free(stats->dirname_end);
  free(stats->word_start);
  free(stats->word_end);
  free(stats->up_case);
  free(stats->low_case);
  free(stats->dotfile);
}

rda(1) void stats_print(const stats_t *stats, uint count) {
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
  printf(", up_case: [");
  for(size_t i = 0; i < count - 1; i++) {
    printf("%u, ", stats->up_case[i]);
  }
  printf("%u]", stats->up_case[count - 1]);
  printf(", low_case: [");
  for(size_t i = 0; i < count - 1; i++) {
    printf("%u, ", stats->low_case[i]);
  }
  printf("%u]", stats->low_case[count - 1]);
  printf(", dotfile: [");
  for(size_t i = 0; i < count - 1; i++) {
    printf(stats->dotfile[i] ? "true, " : "false, ");
  }
  printf(stats->dotfile[count - 1] ? "true]" : "false]");
  printf("}");
}

rda(4) void stats(stats_t *stats, uint *ranges, uint rangesl,
                  const char *const *expr, const char *str) {
  assert(ranges);
  assert(rangesl > 0);
  assert(str);
  assert(str[0]);
  assert(stats);
  assert(stats->dirname_start);
  assert(stats->dirname_end);
  assert(stats->word_start);
  assert(stats->word_end);
  assert(stats->up_case);
  assert(stats->low_case);
  assert(stats->dotfile);

  stats->depth = depth(str);

  dirname_start_distance(ranges, rangesl, stats->dirname_start, str);
  dirname_end_distance(ranges, rangesl, stats->dirname_end, str);
  word_start_distance(ranges, rangesl, stats->word_start, str);
  word_end_distance(ranges, rangesl, stats->word_end, str);
  up_case_count(ranges, rangesl, stats->up_case, str, expr);
  low_case_count(ranges, rangesl, stats->low_case, str, expr);
  dotfile(ranges, rangesl, stats->dotfile, str, stats->dirname_start);
}
