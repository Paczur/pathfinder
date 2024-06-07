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
  uchar node_is[] = {1, 2};
  uchar ret;
  dirname_end_distance(node_is, 2, &ret, "pr");
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
TEST(word_end_distance, multiple_alternating) {
  uchar node_is[] = {1, 2, 4, 5, 8, 9};
  uchar ans[3] = {0, 1, 0};
  uchar ret[3];
  word_end_distance(node_is, 6, ret, "pr?prr/pp");
  assert_memory_equal(ret, ans, 3);
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
    ADD(word_end_distance, relative_end),
    ADD(word_end_distance, relative_middle),
    ADD(word_end_distance, multiple_alternating),
  };
  return cmocka_run_group_tests(tests, NULL, NULL);
}
