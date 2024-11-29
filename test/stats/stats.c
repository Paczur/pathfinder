#include "stats.h"
#include <stats/stats.c>

CTF_TEST(depth_relative) { expect_uint_eq(depth("p"), 1); }
CTF_TEST(depth_absolute) { expect_uint_eq(depth("/p/t/e"), 3); }
CTF_TEST(depth_suffix_slash) { expect_uint_eq(depth("p/t/e/"), 3); }
CTF_TEST(depth_root) { expect_uint_eq(depth("/"), 0); }

CTF_TEST(dirname_start_distance_relative_start) {
  uint node_is[] = {0, 1};
  uint ret;
  dirname_start_distance(node_is, 2, &ret, "pr");
  expect_uint_eq(ret, 0);
}
CTF_TEST(dirname_start_distance_relative_middle) {
  uint node_is[] = {2, 3};
  uint ret;
  dirname_start_distance(node_is, 2, &ret, "test");
  expect_uint_eq(ret, 2);
}
CTF_TEST(dirname_start_distance_absolute_start) {
  uint node_is[] = {1, 2};
  uint ret;
  dirname_start_distance(node_is, 2, &ret, "/p");
  expect_uint_eq(ret, 0);
}
CTF_TEST(dirname_start_distance_multiple_alternating) {
  uint node_is[] = {0, 1, 4, 5, 7, 8};
  uint ans[3] = {0, 1, 0};
  uint ret[3] = {0};
  dirname_start_distance(node_is, 6, ret, "pr/prr/pp");
  expect_array_uint_eq(ret, ans);
}

CTF_TEST(dirname_end_distance_relative_end) {
  uint node_is[] = {1, 3};
  uint ret;
  dirname_end_distance(node_is, 2, &ret, "ppr");
  expect_uint_eq(ret, 0);
}
CTF_TEST(dirname_end_distance_relative_middle) {
  uint node_is[] = {1, 2};
  uint ret;
  dirname_end_distance(node_is, 2, &ret, "test");
  expect_uint_eq(ret, 2);
}
CTF_TEST(dirname_end_distance_multiple_alternating) {
  uint node_is[] = {1, 2, 4, 5, 8, 9};
  uint ans[3] = {0, 1, 0};
  uint ret[3] = {0};
  dirname_end_distance(node_is, 6, ret, "pr/prr/pp");
  expect_array_uint_eq(ret, ans);
}

CTF_TEST(word_start_distance_relative_start) {
  uint node_is[] = {0, 1};
  uint ret;
  word_start_distance(node_is, 2, &ret, "pr");
  expect_uint_eq(ret, 0);
}
CTF_TEST(word_start_distance_relative_middle) {
  uint node_is[] = {2, 3};
  uint ret;
  word_start_distance(node_is, 2, &ret, "test");
  expect_uint_eq(ret, 2);
}
CTF_TEST(word_start_distance_absolute_start) {
  uint node_is[] = {1, 2};
  uint ret;
  word_start_distance(node_is, 2, &ret, "/p");
  expect_uint_eq(ret, 0);
}
CTF_TEST(word_start_distance_multiple_alternating) {
  uint node_is[] = {0, 1, 4, 5, 7, 8};
  uint ans[3] = {0, 1, 0};
  uint ret[3] = {0};
  word_start_distance(node_is, 6, ret, "pr/prr-pp");
  expect_array_uint_eq(ret, ans);
}
CTF_TEST(word_start_distance_a) {
  uint node_is[] = {1, 2};
  uint ret;
  word_start_distance(node_is, 2, &ret, "/a");
  expect_uint_eq(ret, 0);
}
CTF_TEST(word_start_distance_A) {
  uint node_is[] = {1, 2};
  uint ret;
  word_start_distance(node_is, 2, &ret, "/A");
  expect_uint_eq(ret, 0);
}
CTF_TEST(word_start_distance_z) {
  uint node_is[] = {1, 2};
  uint ret;
  word_start_distance(node_is, 2, &ret, "/z");
  expect_uint_eq(ret, 0);
}
CTF_TEST(word_start_distance_Z) {
  uint node_is[] = {1, 2};
  uint ret;
  word_start_distance(node_is, 2, &ret, "/Z");
  expect_uint_eq(ret, 0);
}

CTF_TEST(word_end_distance_relative_end) {
  uint node_is[] = {1, 2};
  uint ret;
  word_end_distance(node_is, 2, &ret, "pr");
  expect_uint_eq(ret, 0);
}
CTF_TEST(word_end_distance_relative_middle) {
  uint node_is[] = {1, 2};
  uint ret;
  word_end_distance(node_is, 2, &ret, "test");
  expect_uint_eq(ret, 2);
}
CTF_TEST(word_end_distance_relative_start) {
  uint node_is[] = {0, 1};
  uint ret;
  word_end_distance(node_is, 2, &ret, "test");
  expect_uint_eq(ret, 3);
}
CTF_TEST(word_end_distance_multiple_alternating) {
  uint node_is[] = {1, 2, 4, 5, 8, 9};
  uint ans[3] = {0, 1, 0};
  uint ret[3];
  word_end_distance(node_is, 6, ret, "pr?prr/pp");
  expect_array_uint_eq(ret, ans);
}
CTF_TEST(word_end_distance_a) {
  uint node_is[] = {0, 1};
  uint ret;
  word_end_distance(node_is, 2, &ret, "aa/");
  expect_uint_eq(ret, 1);
}
CTF_TEST(word_end_distance_A) {
  uint node_is[] = {0, 1};
  uint ret;
  word_end_distance(node_is, 2, &ret, "AA/");
  expect_uint_eq(ret, 1);
}
CTF_TEST(word_end_distance_z) {
  uint node_is[] = {0, 1};
  uint ret;
  word_end_distance(node_is, 2, &ret, "zz/");
  expect_uint_eq(ret, 1);
}
CTF_TEST(word_end_distance_Z) {
  uint node_is[] = {0, 1};
  uint ret;
  word_end_distance(node_is, 2, &ret, "ZZ/");
  expect_uint_eq(ret, 1);
}

CTF_TEST(up_case_count_long) {
  uint ranges[] = {1, 4};
  uint ret;
  const char *expr[] = {"res"};
  up_case_count(ranges, 2, &ret, "pressed", expr);
  expect_uint_eq(ret, 0);
}
CTF_TEST(up_case_count_single) {
  uint ranges[] = {0, 1};
  uint ret;
  const char *expr[] = {"t"};
  up_case_count(ranges, 2, &ret, "test", expr);
  expect_uint_eq(ret, 0);
}
CTF_TEST(up_case_count_multiple) {
  uint ranges[] = {1, 3, 5, 6};
  uint ans[] = {0, 0};
  uint ret[2];
  const char *expr[] = {"es", "l"};
  up_case_count(ranges, 4, ret, "test/lol", expr);
  expect_array_uint_eq(ret, ans);
}
CTF_TEST(up_case_count_half) {
  uint ranges[] = {0, 4, 5, 7};
  uint ans[] = {2, 1};
  uint ret[2];
  const char *expr[] = {"test", "lo"};
  up_case_count(ranges, 4, ret, "tEsT/Lol", expr);
  expect_array_uint_eq(ret, ans);
}

CTF_TEST(low_case_count_long) {
  uint ranges[] = {1, 4};
  uint ret;
  const char *expr[] = {"res"};
  low_case_count(ranges, 2, &ret, "pressed", expr);
  expect_uint_eq(ret, 0);
}
CTF_TEST(low_case_count_single) {
  uint ranges[] = {0, 1};
  uint ret;
  const char *expr[] = {"t"};
  low_case_count(ranges, 2, &ret, "test", expr);
  expect_uint_eq(ret, 0);
}
CTF_TEST(low_case_count_multiple) {
  uint ranges[] = {1, 3, 5, 6};
  uint ans[] = {0, 0};
  uint ret[2];
  const char *expr[] = {"es", "l"};
  low_case_count(ranges, 4, ret, "test/lol", expr);
  expect_array_uint_eq(ret, ans);
}
CTF_TEST(low_case_count_half) {
  uint ranges[] = {0, 4, 5, 7};
  uint ans[] = {2, 1};
  uint ret[2];
  const char *expr[] = {"tEsT", "Lo"};
  low_case_count(ranges, 4, ret, "test/lol", expr);
  expect_array_uint_eq(ret, ans);
}

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
  depth_relative,
  depth_absolute,
  depth_suffix_slash,
  depth_root,
  dirname_start_distance_relative_start,
  dirname_start_distance_relative_middle,
  dirname_start_distance_absolute_start,
  dirname_start_distance_multiple_alternating,
  dirname_end_distance_relative_end,
  dirname_end_distance_relative_middle,
  dirname_end_distance_multiple_alternating,
  word_start_distance_relative_start,
  word_start_distance_relative_middle,
  word_start_distance_absolute_start,
  word_start_distance_multiple_alternating,
  word_start_distance_a,
  word_start_distance_A,
  word_start_distance_z,
  word_start_distance_Z,
  word_end_distance_relative_start,
  word_end_distance_relative_end,
  word_end_distance_relative_middle,
  word_end_distance_multiple_alternating,
  word_end_distance_a,
  word_end_distance_A,
  word_end_distance_z,
  word_end_distance_Z,
  up_case_count_long,
  up_case_count_single,
  up_case_count_multiple,
  up_case_count_half,
  low_case_count_long,
  low_case_count_single,
  low_case_count_multiple,
  low_case_count_half,
  stats_single,
  stats_multiple,
};
