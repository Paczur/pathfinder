#include "parse.c"
#include "test.h"

TEST(node_count, relative_space) { assert_int_equal(node_count("p r"), 2); }
TEST(node_count, relative_slash) { assert_int_equal(node_count("p/r"), 2); }
TEST(node_count, absolute) { assert_int_equal(node_count("/p"), 1); }
TEST(node_count, root) { assert_int_equal(node_count("/"), 0); }
TEST(node_count, suffix_slash) { assert_int_equal(node_count("p/"), 1); }

int main(void) {
  const struct CMUnitTest tests[] = {
    ADD(node_count, relative_space), ADD(node_count, relative_slash),
    ADD(node_count, absolute),       ADD(node_count, root),
    ADD(node_count, suffix_slash),
  };
  return cmocka_run_group_tests(tests, NULL, NULL);
}
