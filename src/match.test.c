#include "match.c"
#include "test.h"

TEST(letter, a) { assert_true(letter('a')); }
TEST(letter, z) { assert_true(letter('z')); }
TEST(letter, A) { assert_true(letter('A')); }
TEST(letter, Z) { assert_true(letter('Z')); }
TEST(letter, number) { assert_false(letter('1')); }
TEST(letter, between_lower_and_upper) { assert_false(letter('^')); }

TEST(uppercase, exact) { assert_int_equal(uppercase('A'), 'A'); }
TEST(uppercase, lower) { assert_int_equal(uppercase('a'), 'A'); }

TEST(equal, exact) { assert_true(equal('a', 'a')); }
TEST(equal, lower) { assert_true(equal('A', 'a')); }
TEST(equal, upper) { assert_true(equal('a', 'A')); }
TEST(equal, symbol) { assert_false(equal('~', '^')); }

#define TEST_NODE_MATCHES(name, expr, str, ...)     \
  TEST(node_matches, name) {                        \
    uchar res[] = {__VA_ARGS__};                    \
    uchar range[sizeof(res)];                       \
    assert_true(node_matches(expr, str, range));    \
    assert_memory_equal(range, res, sizeof(range)); \
  }
TEST_NODE_MATCHES(prefix, "pr", "epr", 1, 3)
TEST_NODE_MATCHES(suffix, "pr", "epr", 1, 3)
TEST_NODE_MATCHES(infix, "pr", "epre", 1, 3)
TEST_NODE_MATCHES(exact, "pr", "pr", 0, 2)
TEST_NODE_MATCHES(prefix_repetition, "pr", "ppr", 1, 3)
TEST_NODE_MATCHES(case, "PR", "pr", 0, 2);
TEST_NODE_MATCHES(slash, "pr/test", "pr", 0, 2)
TEST_NODE_MATCHES(space, "pr test2", "pr", 0, 2)
TEST(node_matches, expr_shorter_than_path) {
  assert_false(node_matches("s", "path", NULL));
}

#define TEST_MATCHES(name, expr, str, ...)          \
  TEST(matches, name) {                             \
    uchar res[] = {__VA_ARGS__};                    \
    uchar range[sizeof(res)];                       \
    assert_true(matches(expr, str, range));         \
    assert_memory_equal(range, res, sizeof(range)); \
  }
TEST_MATCHES(space_direct, "p r", "projects/real", 0, 1, 9, 10)
TEST_MATCHES(space_indirect, "p r", "projects/no/real", 0, 1, 12, 13)
TEST_MATCHES(slash, "p/r", "projects/real", 0, 1, 9, 10)
TEST_MATCHES(middle, "e", "project/test", 4, 5)
TEST_MATCHES(absolute, "/p", "/rp", 2, 3)
TEST_MATCHES(absolute_middle, "/p s", "/rp/test", 2, 3, 6, 7)
TEST_MATCHES(second, "p", "test/project", 5, 6)
TEST_MATCHES(absolute_space, "/ p", "/test/pro", 6, 7)
TEST(matches, first_false) { assert_false(matches("pr", "test", NULL)); }
TEST(matches, absolute_false) { assert_false(matches("/p", "/test", NULL)); }
TEST(matches, space_false) {
  assert_false(matches("p r", "projects/no/luck", NULL));
}
TEST(matches, absolute_relative) { assert_false(matches("/p", "pr", NULL)); }
TEST(matches, absolute_indirect) {
  assert_false(matches("/p", "/test/pro", NULL));
}
TEST(matches, slash_false) {
  assert_false(matches("p/r", "projects/no", NULL));
}

int main(void) {
  const struct CMUnitTest tests[] = {
    ADD(letter, a),
    ADD(letter, A),
    ADD(letter, z),
    ADD(letter, Z),
    ADD(letter, number),
    ADD(letter, between_lower_and_upper),
    ADD(uppercase, exact),
    ADD(uppercase, lower),
    ADD(equal, exact),
    ADD(equal, lower),
    ADD(equal, upper),
    ADD(equal, symbol),
    ADD(node_matches, suffix),
    ADD(node_matches, prefix),
    ADD(node_matches, infix),
    ADD(node_matches, exact),
    ADD(node_matches, prefix_repetition),
    ADD(node_matches, case),
    ADD(node_matches, slash),
    ADD(node_matches, space),
    ADD(node_matches, expr_shorter_than_path),
    ADD(matches, space_direct),
    ADD(matches, space_indirect),
    ADD(matches, space_false),
    ADD(matches, slash),
    ADD(matches, first_false),
    ADD(matches, slash_false),
    ADD(matches, absolute),
    ADD(matches, second),
    ADD(matches, absolute_indirect),
    ADD(matches, middle),
    ADD(matches, absolute_middle),
    ADD(matches, absolute_false),
    ADD(matches, absolute_space),
    ADD(matches, absolute_relative),
  };
  return cmocka_run_group_tests(tests, NULL, NULL);
}
