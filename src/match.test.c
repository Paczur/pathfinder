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

#define TEST_NODE_MATCHES(name, str, expr, ...)     \
  TEST(node_matches, name) {                        \
    uchar res[] = {__VA_ARGS__};                    \
    uchar range[sizeof(res)];                       \
    assert_true(node_matches(range, str, expr));    \
    assert_memory_equal(range, res, sizeof(range)); \
  }
TEST_NODE_MATCHES(prefix, "epr", "pr", 1, 3)
TEST_NODE_MATCHES(suffix, "epr", "pr", 1, 3)
TEST_NODE_MATCHES(infix, "epre", "pr", 1, 3)
TEST_NODE_MATCHES(exact, "pr", "pr", 0, 2)
TEST_NODE_MATCHES(prefix_repetition, "ppr", "pr", 1, 3)
TEST_NODE_MATCHES(case, "PR", "pr", 0, 2);
TEST_NODE_MATCHES(slash, "pr", "pr/test", 0, 2)
TEST_NODE_MATCHES(empty, "pr", "", 1, 1)
TEST_NODE_MATCHES(space, "pr test", "pr t", 0, 4)
TEST(node_matches, space_false) {
  assert_false(node_matches(NULL, "pr", "pr test"));
}
TEST(node_matches, expr_shorter_than_path) {
  assert_false(node_matches(NULL, "path", "s"));
}

#define TEST_MATCHES(name, str, expr, ...)          \
  TEST(matches, name) {                             \
    uchar res[] = {__VA_ARGS__};                    \
    uchar range[sizeof(res)];                       \
    char *e[] = {expr};                             \
    assert_true(matches(range, str, e, 1));         \
    assert_memory_equal(range, res, sizeof(range)); \
  }
#define TEST_NOT_MATCHES(name, str, ...)                           \
  TEST(matches, name) {                                            \
    char *e[] = {__VA_ARGS__};                                     \
    assert_false(matches(NULL, str, e, sizeof(e) / sizeof(e[0]))); \
  }

TEST(matches, space_direct) {
  uchar res[] = {0, 1, 9, 10};
  uchar range[sizeof(res)];
  char *expr[] = {"p", "r"};
  assert_true(matches(range, "projects/real", expr, sizeof(expr)));
  assert_memory_equal(range, res, sizeof(range));
}
TEST(matches, space_indirect) {
  uchar res[] = {0, 1, 12, 13};
  uchar range[sizeof(res)];
  char *expr[] = {"p", "r"};
  assert_true(matches(range, "projects/no/real", expr, sizeof(expr)));
  assert_memory_equal(range, res, sizeof(range));
}
TEST_MATCHES(slash, "projects/real", "p/r", 0, 1, 9, 10)
TEST_MATCHES(middle, "project", "e", 4, 5)
TEST_MATCHES(absolute, "/rp", "/p", 2, 3)
TEST(matches, absolute_middle) {
  uchar res[] = {2, 3, 6, 7};
  uchar range[sizeof(res)];
  char *e[] = {"/p", "s"};
  assert_true(matches(range, "/rp/test", e, 2));
  assert_memory_equal(range, res, sizeof(range));
}
TEST(matches, absolute_space) {
  uchar res[] = {0, 1, 6, 7};
  uchar range[sizeof(res)];
  char *e[] = {"/", "p"};
  assert_true(matches(range, "/test/pro", e, 2));
  assert_memory_equal(range, res, sizeof(range));
}
TEST_MATCHES(second, "test/project", "p", 5, 6)
TEST_MATCHES(skip_one, "projects/lol/test", "p//t", 0, 1, 11, 11, 13, 14)
TEST_MATCHES(always_skips, "projects/test/test2", "p//t", 0, 1, 12, 12, 14, 15)
TEST_NOT_MATCHES(first_false, "test", "pr")
TEST_NOT_MATCHES(absolute_false, "/test", "/p")
TEST_NOT_MATCHES(longer_path, "projects/test", "p")
TEST_NOT_MATCHES(space_false, "projects/no/luck", "p r")
TEST_NOT_MATCHES(absolute_relative, "pr", "/p")
TEST_NOT_MATCHES(absolute_indirect, "/test/pro", "/p")
TEST_NOT_MATCHES(slash_false, "projects/no", "p/r")

int main(void) {
  const struct CMUnitTest tests[] = {
    ADD(letter, a),
    ADD(letter, A),
    ADD(letter, between_lower_and_upper),
    ADD(letter, number),
    ADD(letter, z),
    ADD(letter, Z),
    ADD(uppercase, exact),
    ADD(uppercase, lower),
    ADD(equal, exact),
    ADD(equal, lower),
    ADD(equal, symbol),
    ADD(equal, upper),
    ADD(node_matches, case),
    ADD(node_matches, empty),
    ADD(node_matches, exact),
    ADD(node_matches, expr_shorter_than_path),
    ADD(node_matches, infix),
    ADD(node_matches, prefix),
    ADD(node_matches, prefix_repetition),
    ADD(node_matches, slash),
    ADD(node_matches, space),
    ADD(node_matches, space_false),
    ADD(node_matches, suffix),
    ADD(matches, absolute),
    ADD(matches, absolute_false),
    ADD(matches, absolute_indirect),
    ADD(matches, absolute_middle),
    ADD(matches, absolute_relative),
    ADD(matches, absolute_space),
    ADD(matches, always_skips),
    ADD(matches, first_false),
    ADD(matches, longer_path),
    ADD(matches, middle),
    ADD(matches, second),
    ADD(matches, skip_one),
    ADD(matches, slash),
    ADD(matches, slash_false),
    ADD(matches, space_direct),
    ADD(matches, space_false),
    ADD(matches, space_indirect),
  };
  return cmocka_run_group_tests(tests, NULL, NULL);
}
