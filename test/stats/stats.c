#include "stats.h"
#include <stats/stats.h>

static void expect_stats_eq(const stats_t *x, const stats_t *y, uint count) {
  expect_non_null(x->dirname_start);
  expect_non_null(y->dirname_start);
  expect_non_null(x->dirname_end);
  expect_non_null(y->dirname_end);
  expect_non_null(x->word_start);
  expect_non_null(y->word_start);
  expect_non_null(x->word_end);
  expect_non_null(y->word_end);
  expect_non_null(x->up_case);
  expect_non_null(y->up_case);
  expect_non_null(x->low_case);
  expect_non_null(y->low_case);
  expect_uint_neq(x->depth, 0);
  expect_uint_neq(y->depth, 0);
  expect_uint_eq(x->depth, y->depth);
  expect_memory_uint_eq(x->dirname_start, y->dirname_start, count);
  expect_memory_uint_eq(x->dirname_end, y->dirname_end, count);
  expect_memory_uint_eq(x->word_start, y->word_start, count);
  expect_memory_uint_eq(x->word_end, y->word_end, count);
  expect_memory_uint_eq(x->up_case, y->up_case, count);
  expect_memory_uint_eq(x->low_case, y->low_case, count);
  expect_memory_int_eq(x->dotfile, y->dotfile, count);
}

CTF_TEST(stats_single) {
  stats_t st = STATS_INIT(1);
  uint ranges[2] = {1, 5};
  stats_t ans = {.depth = 1,
                 .dirname_start = (uint[]){1},
                 .dirname_end = (uint[]){0},
                 .word_start = (uint[]){1},
                 .word_end = (uint[]){0},
                 .up_case = (uint[]){0},
                 .low_case = (uint[]){0},
                 .dotfile = (bool[]){false}};
  const char *expr[] = {"test"};
  stats(&st, ranges, 2, expr, "ttest");
  expect_stats_eq(&st, &ans, 1);
}

CTF_TEST(stats_multiple) {
  stats_t st = STATS_INIT(3);
  uint ranges[] = {1, 5, 8, 10, 11, 12};
  stats_t ans = {.depth = 4,
                 .dirname_start = (uint[]){1, 0, 0},
                 .dirname_end = (uint[]){0, 0, 3},
                 .word_start = (uint[]){1, 0, 0},
                 .word_end = (uint[]){0, 0, 3},
                 .up_case = (uint[]){0, 1, 0},
                 .low_case = (uint[]){1, 0, 0},
                 .dotfile = (bool[]){false, false, false}};
  const char *expr[] = {"Test", "ro/a"};
  stats(&st, ranges, 6, expr, "ttest/p/Ro/afgd");
  expect_stats_eq(&st, &ans, 3);
}

CTF_GROUP(stats_group) = {
  stats_single,
  stats_multiple,
};
