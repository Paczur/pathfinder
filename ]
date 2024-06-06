#include "stat.c"
#include "test.h"

TEST(depth, relative) { assert_int_equal(depth("p"), 1); }
TEST(depth, absolute) { assert_int_equal(depth("/p/t/e"), 3); }
TEST(depth, suffix_slash) { assert_int_equal(depth("p/t/e/"), 3); }
TEST(depth, root) { assert_int_equal(depth("/"), 0); }

TEST(dirname_start_distance, relative_start) {
  uchar node_is = 0;
  uchar *ret = dirname_start_distance(&node_is, 1, "pr");
  assert_int_equal(ret[0], 0);
  free(ret);
}
TEST(dirname_start_distance, relative_middle) {
  uchar node_is = 2;
  uchar *ret = dirname_start_distance(&node_is, 1, "test");
  assert_int_equal(ret[0], 2);
  free(ret);
}

int main(void) {
  const struct CMUnitTest tests[] = {
    ADD(depth, relative),
    ADD(depth, absolute),
    ADD(depth, suffix_slash),
    ADD(depth, root),
    ADD(dirname_start_distance, relative_start),
    ADD(dirname_start_distance, relative_middle),
  };
  return cmocka_run_group_tests(tests, NULL, NULL);
}
