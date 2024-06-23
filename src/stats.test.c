#include "stats.c"
#include "test.h"

TEST(depth, relative) { assert_int_equal(depth("p"), 1); }
TEST(depth, absolute) { assert_int_equal(depth("/p/t/e"), 3); }
TEST(depth, suffix_slash) { assert_int_equal(depth("p/t/e/"), 3); }
TEST(depth, root) { assert_int_equal(depth("/"), 0); }

TEST(dirname_start_distance, relative_start) {
  uint node_is[] = {0, 1};
  uint ret;
  dirname_start_distance(node_is, 2, &ret, "pr");
  assert_int_equal(ret, 0);
}
TEST(dirname_start_distance, relative_middle) {
  uint node_is[] = {2, 3};
  uint ret;
  dirname_start_distance(node_is, 2, &ret, "test");
  assert_int_equal(ret, 2);
}
TEST(dirname_start_distance, absolute_start) {
  uint node_is[] = {1, 2};
  uint ret;
  dirname_start_distance(node_is, 2, &ret, "/p");
  assert_int_equal(ret, 0);
}
TEST(dirname_start_distance, multiple_alternating) {
  uint node_is[] = {0, 1, 4, 5, 7, 8};
  uint ans[3] = {0, 1, 0};
  uint ret[3];
  dirname_start_distance(node_is, 6, ret, "pr/prr/pp");
  assert_memory_equal(ret, ans, 3);
}

TEST(dirname_end_distance, relative_end) {
  uint node_is[] = {1, 3};
  uint ret;
  dirname_end_distance(node_is, 2, &ret, "ppr");
  assert_int_equal(ret, 0);
}
TEST(dirname_end_distance, relative_middle) {
  uint node_is[] = {1, 2};
  uint ret;
  dirname_end_distance(node_is, 2, &ret, "test");
  assert_int_equal(ret, 2);
}
TEST(dirname_end_distance, multiple_alternating) {
  uint node_is[] = {1, 2, 4, 5, 8, 9};
  uint ans[3] = {0, 1, 0};
  uint ret[3];
  dirname_end_distance(node_is, 6, ret, "pr/prr/pp");
  assert_memory_equal(ret, ans, 3);
}

TEST(word_start_distance, relative_start) {
  uint node_is[] = {0, 1};
  uint ret;
  word_start_distance(node_is, 2, &ret, "pr");
  assert_int_equal(ret, 0);
}
TEST(word_start_distance, relative_middle) {
  uint node_is[] = {2, 3};
  uint ret;
  word_start_distance(node_is, 2, &ret, "test");
  assert_int_equal(ret, 2);
}
TEST(word_start_distance, absolute_start) {
  uint node_is[] = {1, 2};
  uint ret;
  word_start_distance(node_is, 2, &ret, "/p");
  assert_int_equal(ret, 0);
}
TEST(word_start_distance, multiple_alternating) {
  uint node_is[] = {0, 1, 4, 5, 7, 8};
  uint ans[3] = {0, 1, 0};
  uint ret[3];
  word_start_distance(node_is, 6, ret, "pr/prr-pp");
  assert_memory_equal(ret, ans, 3);
}
TEST(word_start_distance, a) {
  uint node_is[] = {1, 2};
  uint ret;
  word_start_distance(node_is, 2, &ret, "/a");
  assert_int_equal(ret, 0);
}
TEST(word_start_distance, A) {
  uint node_is[] = {1, 2};
  uint ret;
  word_start_distance(node_is, 2, &ret, "/A");
  assert_int_equal(ret, 0);
}
TEST(word_start_distance, z) {
  uint node_is[] = {1, 2};
  uint ret;
  word_start_distance(node_is, 2, &ret, "/z");
  assert_int_equal(ret, 0);
}
TEST(word_start_distance, Z) {
  uint node_is[] = {1, 2};
  uint ret;
  word_start_distance(node_is, 2, &ret, "/Z");
  assert_int_equal(ret, 0);
}

TEST(word_end_distance, relative_end) {
  uint node_is[] = {1, 2};
  uint ret;
  word_end_distance(node_is, 2, &ret, "pr");
  assert_int_equal(ret, 0);
}
TEST(word_end_distance, relative_middle) {
  uint node_is[] = {1, 2};
  uint ret;
  word_end_distance(node_is, 2, &ret, "test");
  assert_int_equal(ret, 2);
}
TEST(word_end_distance, relative_start) {
  uint node_is[] = {0, 1};
  uint ret;
  word_end_distance(node_is, 2, &ret, "test");
  assert_int_equal(ret, 3);
}
TEST(word_end_distance, multiple_alternating) {
  uint node_is[] = {1, 2, 4, 5, 8, 9};
  uint ans[3] = {0, 1, 0};
  uint ret[3];
  word_end_distance(node_is, 6, ret, "pr?prr/pp");
  assert_memory_equal(ret, ans, 3);
}
TEST(word_end_distance, a) {
  uint node_is[] = {0, 1};
  uint ret;
  word_end_distance(node_is, 2, &ret, "aa/");
  assert_int_equal(ret, 1);
}
TEST(word_end_distance, A) {
  uint node_is[] = {0, 1};
  uint ret;
  word_end_distance(node_is, 2, &ret, "AA/");
  assert_int_equal(ret, 1);
}
TEST(word_end_distance, z) {
  uint node_is[] = {0, 1};
  uint ret;
  word_end_distance(node_is, 2, &ret, "zz/");
  assert_int_equal(ret, 1);
}
TEST(word_end_distance, Z) {
  uint node_is[] = {0, 1};
  uint ret;
  word_end_distance(node_is, 2, &ret, "ZZ/");
  assert_int_equal(ret, 1);
}

TEST(bad_case_count, long) {
  uint ranges[] = {1, 4};
  uint ret;
  char *expr[] = {"res"};
  bad_case_count(ranges, 2, &ret, "pressed", expr);
  assert_int_equal(ret, 0);
}
TEST(bad_case_count, single) {
  uint ranges[] = {0, 1};
  uint ret;
  char *expr[] = {"t"};
  bad_case_count(ranges, 2, &ret, "test", expr);
  assert_int_equal(ret, 0);
}
TEST(bad_case_count, multiple) {
  uint ranges[] = {1, 3, 5, 6};
  uint ans[] = {0, 0};
  uint ret[2];
  char *expr[] = {"es", "l"};
  bad_case_count(ranges, 4, ret, "test/lol", expr);
  assert_memory_equal(ret, ans, 2);
}
TEST(bad_case_count, half) {
  uint ranges[] = {0, 4, 5, 7};
  uint ans[] = {2, 1};
  uint ret[2];
  char *expr[] = {"test", "lo"};
  bad_case_count(ranges, 4, ret, "tEsT/Lol", expr);
  assert_memory_equal(ret, ans, 2);
}

void assert_stats_equal(const stats_t *x, const stats_t *y, uint count) {
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
  assert_int_equal(x->depth, y->depth);
  assert_memory_equal(x->dirname_start, y->dirname_start,
                      count * sizeof(x->dirname_start[0]));
  assert_memory_equal(x->dirname_end, y->dirname_end,
                      count * sizeof(x->dirname_end[0]));
  assert_memory_equal(x->word_start, y->word_start,
                      count * sizeof(x->word_start[0]));
  assert_memory_equal(x->word_end, y->word_end, count * sizeof(x->word_end[0]));
  assert_memory_equal(x->bad_case, y->bad_case, count * sizeof(x->bad_case[0]));
  assert_memory_equal(x->dotfile, y->dotfile, count * sizeof(x->dotfile[0]));
}

TEST(stats, single) {
  stats_t st = STATS_INIT(1);
  uint ranges[2] = {1, 5};
  stats_t ans = {.depth = 1,
                 .dirname_start = &(uint[]){1},
                 .dirname_end = &(uint[]){0},
                 .word_start = &(uint[]){1},
                 .word_end = &(uint[]){0},
                 .bad_case = &(uint[]){0},
                 .dotfile = &(bool[]){false}};
  char *expr[] = {"test"};
  stats(&st, ranges, 2, expr, "ttest");
  assert_stats_equal(&st, &ans, 1);
}

TEST(stats, multiple) {
  stats_t st = STATS_INIT(3);
  uint ranges[] = {1, 5, 8, 10, 11, 12};
  stats_t ans = {.depth = 4,
                 .dirname_start = (uint[]){1, 0, 0},
                 .dirname_end = (uint[]){0, 0, 3},
                 .word_start = (uint[]){1, 0, 0},
                 .word_end = (uint[]){0, 0, 3},
                 .bad_case = (uint[]){1, 0, 0},
                 .dotfile = &(bool[]){false}};
  char *expr[] = {"Test", "ro/a"};
  stats(&st, ranges, 6, expr, "ttest/p/ro/afgd");
  assert_stats_equal(&st, &ans, 3);
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
    ADD(stats, single),
    ADD(stats, multiple),
  };
  return cmocka_run_group_tests(tests, NULL, NULL);
}
