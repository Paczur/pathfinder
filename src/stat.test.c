#include "stat.c"
#include "test.h"

TEST(depth, relative) { assert_int_equal(depth("p"), 1); }
TEST(depth, absolute) { assert_int_equal(depth("/p/t/e"), 3); }
TEST(depth, suffix_slash) { assert_int_equal(depth("p/t/e/"), 3); }
TEST(depth, root) { assert_int_equal(depth("/"), 0); }

int main(void) {
  const struct CMUnitTest tests[] = {
    ADD(depth, relative),
    ADD(depth, absolute),
    ADD(depth, suffix_slash),
    ADD(depth, root),
  };
  return cmocka_run_group_tests(tests, NULL, NULL);
}
