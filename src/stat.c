#include "stat.h"
#include "match.h"
#include <assert.h>
#include <stdlib.h>
#include <stdio.h>

static uchar depth(const char *str) {
  assert(str);
  assert(str[0]);
  uchar counter = 1;
  if(!str[1]) return str[0] != '/';
  for(size_t i = 1; str[i + 1]; i++) {
    if(str[i] == '/') counter++;
  }
  return counter;
}

static void dirname_start_distance(const uchar *ranges, uchar rangesl,
                                   uchar *ret, const char *str) {
  assert(rangesl > 0);
  assert(ranges);
  assert(str);
  assert(str[0]);
  assert(ret);
  uchar dist;
  uchar c;
  for(size_t i = 0; i < rangesl / 2; i++) {
    dist = 0;
    c = ranges[i * 2];
    for(; c >= dist && str[c - dist] != '/'; dist++) {
    }
    ret[i] = dist - 1;
  }
}

static void dirname_end_distance(const uchar *ranges, uchar rangesl, uchar *ret,
                                 const char *str) {
  assert(rangesl > 0);
  assert(ranges);
  assert(str);
  assert(str[0]);
  assert(ret);
  uchar dist;
  uchar c;
  for(size_t i = 0; i < rangesl / 2; i++) {
    dist = 0;
    c = ranges[i * 2 + 1];
    for(; str[c + dist] && str[c + dist] != '/'; dist++) {
    }
    ret[i] = dist;
  }
}

static void word_start_distance(const uchar *ranges, uchar rangesl, uchar *ret,
                                const char *str) {
  assert(rangesl > 0);
  assert(ranges);
  assert(str);
  assert(str[0]);
  assert(ret);
  uchar dist;
  uchar c;
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

static void word_end_distance(const uchar *ranges, uchar rangesl, uchar *ret,
                              const char *str) {
  assert(rangesl > 0);
  assert(ranges);
  assert(str);
  assert(str[0]);
  assert(ret);
  uchar dist;
  uchar c;
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

static void bad_case_count(const uchar *ranges, uchar rangesl, uchar *ret,
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
      if(expr[j][i] != str[ranges[ranges_i] + (slash ? i - slash : i)]) {
        counter++;
      }
    }
    ret[index++] = counter;
    counter = 0;
    ranges_i += 2;
    slash = 0;
  }
}

void stat(stats_t *stats, uchar *ranges, uchar rangesl, const char *const *expr,
          const char *str) {
  assert(ranges);
  assert(rangesl > 0);
  assert(str);
  assert(str[0]);
  assert(stats);
  assert(stats->dirname_start);
  assert(stats->dirname_end);
  assert(stats->word_start);
  assert(stats->word_end);
  assert(stats->bad_case);

  stats->depth = depth(str);
  stats->count = rangesl / 2;

  dirname_start_distance(ranges, rangesl, stats->dirname_start, str);
  dirname_end_distance(ranges, rangesl, stats->dirname_end, str);
  word_start_distance(ranges, rangesl, stats->word_start, str);
  word_end_distance(ranges, rangesl, stats->word_end, str);
  bad_case_count(ranges, rangesl, stats->bad_case, str, expr);
}
