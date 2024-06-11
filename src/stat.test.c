#include "stat.c"
#include "test.h"

TEST(depth, relative) { assert_int_equal(depth("p"), 1); }
TEST(depth, absolute) { assert_int_equal(depth("/p/t/e"), 3); }
TEST(depth, suffix_slash) { assert_int_equal(depth("p/t/e/"), 3); }
TEST(depth, root) { assert_int_equal(depth("/"), 0); }

TEST(dirname_start_distance, relative_start) {
  uchar node_is[] = {0, 1};
  uchar ret;
  dirname_start_distance(node_is, 2, &ret, "pr");
  assert_int_equal(ret, 0);
}
TEST(dirname_start_distance, relative_middle) {
  uchar node_is[] = {2, 3};
  uchar ret;
  dirname_start_distance(node_is, 2, &ret, "test");
  assert_int_equal(ret, 2);
}
TEST(dirname_start_distance, absolute_start) {
  uchar node_is[] = {1, 2};
  uchar ret;
  dirname_start_distance(node_is, 2, &ret, "/p");
  assert_int_equal(ret, 0);
}
TEST(dirname_start_distance, multiple_alternating) {
  uchar node_is[] = {0, 1, 4, 5, 7, 8};
  uchar ans[3] = {0, 1, 0};
  uchar ret[3];
  dirname_start_distance(node_is, 6, ret, "pr/prr/pp");
  assert_memory_equal(ret, ans, 3);
}

TEST(dirname_end_distance, relative_end) {
  uchar node_is[] = {1, 3};
  uchar ret;
  dirname_end_distance(node_is, 2, &ret, "ppr");
  assert_int_equal(ret, 0);
}
TEST(dirname_end_distance, relative_middle) {
  uchar node_is[] = {1, 2};
  uchar ret;
  dirname_end_distance(node_is, 2, &ret, "test");
  assert_int_equal(ret, 2);
}
TEST(dirname_end_distance, multiple_alternating) {
  uchar node_is[] = {1, 2, 4, 5, 8, 9};
  uchar ans[3] = {0, 1, 0};
  uchar ret[3];
  dirname_end_distance(node_is, 6, ret, "pr/prr/pp");
  assert_memory_equal(ret, ans, 3);
}

TEST(word_start_distance, relative_start) {
  uchar node_is[] = {0, 1};
  uchar ret;
  word_start_distance(node_is, 2, &ret, "pr");
  assert_int_equal(ret, 0);
}
TEST(word_start_distance, relative_middle) {
  uchar node_is[] = {2, 3};
  uchar ret;
  word_start_distance(node_is, 2, &ret, "test");
  assert_int_equal(ret, 2);
}
TEST(word_start_distance, absolute_start) {
  uchar node_is[] = {1, 2};
  uchar ret;
  word_start_distance(node_is, 2, &ret, "/p");
  assert_int_equal(ret, 0);
}
TEST(word_start_distance, multiple_alternating) {
  uchar node_is[] = {0, 1, 4, 5, 7, 8};
  uchar ans[3] = {0, 1, 0};
  uchar ret[3];
  word_start_distance(node_is, 6, ret, "pr/prr-pp");
  assert_memory_equal(ret, ans, 3);
}
TEST(word_start_distance, a) {
  uchar node_is[] = {1, 2};
  uchar ret;
  word_start_distance(node_is, 2, &ret, "/a");
  assert_int_equal(ret, 0);
}
TEST(word_start_distance, A) {
  uchar node_is[] = {1, 2};
  uchar ret;
  word_start_distance(node_is, 2, &ret, "/A");
  assert_int_equal(ret, 0);
}
TEST(word_start_distance, z) {
  uchar node_is[] = {1, 2};
  uchar ret;
  word_start_distance(node_is, 2, &ret, "/z");
  assert_int_equal(ret, 0);
}
TEST(word_start_distance, Z) {
  uchar node_is[] = {1, 2};
  uchar ret;
  word_start_distance(node_is, 2, &ret, "/Z");
  assert_int_equal(ret, 0);
}

TEST(word_end_distance, relative_end) {
  uchar node_is[] = {1, 2};
  uchar ret;
  word_end_distance(node_is, 2, &ret, "pr");
  assert_int_equal(ret, 0);
}
TEST(word_end_distance, relative_middle) {
  uchar node_is[] = {1, 2};
  uchar ret;
  word_end_distance(node_is, 2, &ret, "test");
  assert_int_equal(ret, 2);
}
TEST(word_end_distance, relative_start) {
  uchar node_is[] = {0, 1};
  uchar ret;
  word_end_distance(node_is, 2, &ret, "test");
  assert_int_equal(ret, 3);
}
TEST(word_end_distance, multiple_alternating) {
  uchar node_is[] = {1, 2, 4, 5, 8, 9};
  uchar ans[3] = {0, 1, 0};
  uchar ret[3];
  word_end_distance(node_is, 6, ret, "pr?prr/pp");
  assert_memory_equal(ret, ans, 3);
}
TEST(word_end_distance, a) {
  uchar node_is[] = {0, 1};
  uchar ret;
  word_end_distance(node_is, 2, &ret, "aa/");
  assert_int_equal(ret, 1);
}
TEST(word_end_distance, A) {
  uchar node_is[] = {0, 1};
  uchar ret;
  word_end_distance(node_is, 2, &ret, "AA/");
  assert_int_equal(ret, 1);
}
TEST(word_end_distance, z) {
  uchar node_is[] = {0, 1};
  uchar ret;
  word_end_distance(node_is, 2, &ret, "zz/");
  assert_int_equal(ret, 1);
}
TEST(word_end_distance, Z) {
  uchar node_is[] = {0, 1};
  uchar ret;
  word_end_distance(node_is, 2, &ret, "ZZ/");
  assert_int_equal(ret, 1);
}

TEST(bad_case_count, long) {
  uchar ranges[] = {1, 4};
  uchar ret;
  bad_case_count(ranges, 2, &ret, "pressed", "res");
  assert_int_equal(ret, 0);
}
TEST(bad_case_count, single) {
  uchar ranges[] = {0, 1};
  uchar ret;
  bad_case_count(ranges, 2, &ret, "test", "t");
  assert_int_equal(ret, 0);
}
TEST(bad_case_count, multiple) {
  uchar ranges[] = {1, 3, 5, 6};
  uchar ans[] = {0, 0};
  uchar ret[2];
  bad_case_count(ranges, 4, ret, "test/lol", "es l");
  assert_memory_equal(ret, ans, 2);
}
TEST(bad_case_count, half) {
  uchar ranges[] = {0, 4, 5, 7};
  uchar ans[] = {2, 1};
  uchar ret[2];
  bad_case_count(ranges, 4, ret, "tEsT/Lol", "test lo");
  assert_memory_equal(ret, ans, 2);
}

void assert_stats_equal(const stats_t *x, const stats_t *y) {
  assert_non_null(x->dirname_start);
  assert_non_null(y->dirname_start);
  assert_non_null(x->dirname_end);
  assert_non_null(y->dirname_end);
  assert_non_null(x->word_start);
  assert_non_null(y->word_start);
  assert_non_null(x->word_end);
  assert_non_null(y->word_end);
  assert_non_null(x->bad_case);
  assert_non_null(y->bad_case);
  assert_int_not_equal(x->depth, 0);
  assert_int_not_equal(y->depth, 0);
  assert_int_not_equal(x->count, 0);
  assert_int_not_equal(y->count, 0);
  assert_int_equal(x->depth, y->depth);
  assert_int_equal(x->count, y->count);
  assert_memory_equal(x->dirname_start, y->dirname_start, x->count);
  assert_memory_equal(x->dirname_end, y->dirname_end, x->count);
  assert_memory_equal(x->word_start, y->word_start, x->count);
  assert_memory_equal(x->word_end, y->word_end, x->count);
  assert_memory_equal(x->bad_case, y->bad_case, x->count);
}

TEST(stat, single) {
  stats_t stats = STAT_INIT(1);
  uchar ranges[2] = {1, 5};
  stats_t ans = {.depth = 1,
                 .count = 1,
                 .dirname_start = &(uchar[]){1},
                 .dirname_end = &(uchar[]){0},
                 .word_start = &(uchar[]){1},
                 .word_end = &(uchar[]){0},
                 .bad_case = &(uchar[]){0}};
  stat(&stats, ranges, 2, "test", "ttest");
  assert_stats_equal(&stats, &ans);
}

TEST(stat, multiple) {
  stats_t stats = STAT_INIT(3);
  uchar ranges[] = {1, 5, 8, 10, 11, 12};
  stats_t ans = {.depth = 4,
                 .count = 3,
                 .dirname_start = (uchar[]){1, 0, 0},
                 .dirname_end = (uchar[]){0, 0, 3},
                 .word_start = (uchar[]){1, 0, 0},
                 .word_end = (uchar[]){0, 0, 3},
                 .bad_case = (uchar[]){1, 0, 0}};
  stat(&stats, ranges, 6, "Test ro/a", "ttest/p/ro/afgd");
  assert_stats_equal(&stats, &ans);
}

int main(void) {
  const struct CMUnitTest tests[] = {
    ADD(depth, relative),
    ADD(depth, absolute),
    ADD(depth, suffix_slash),
    ADD(depth, root),
    ADD(dirname_start_distance, relative_start),
    ADD(dirname_start_distance, relative_middle),
    ADD(dirname_start_distance, absolute_start),
    ADD(dirname_start_distance, multiple_alternating),
    ADD(dirname_end_distance, relative_end),
    ADD(dirname_end_distance, relative_middle),
    ADD(dirname_end_distance, multiple_alternating),
    ADD(word_start_distance, relative_start),
    ADD(word_start_distance, relative_middle),
    ADD(word_start_distance, absolute_start),
    ADD(word_start_distance, multiple_alternating),
    ADD(word_start_distance, a),
    ADD(word_start_distance, A),
    ADD(word_start_distance, z),
    ADD(word_start_distance, Z),
    ADD(word_end_distance, relative_start),
    ADD(word_end_distance, relative_end),
    ADD(word_end_distance, relative_middle),
    ADD(word_end_distance, multiple_alternating),
    ADD(word_end_distance, a),
    ADD(word_end_distance, A),
    ADD(word_end_distance, z),
    ADD(word_end_distance, Z),
    ADD(bad_case_count, long),
    ADD(bad_case_count, single),
    ADD(bad_case_count, multiple),
    ADD(bad_case_count, half),
    ADD(stat, single),
    ADD(stat, multiple),
    // Make tests for stat function
  };
  return cmocka_run_group_tests(tests, NULL, NULL);
}
