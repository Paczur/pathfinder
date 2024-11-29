#include "match.h"
#include <match/match.c>

#define LENGTH(a) (sizeof(a) / sizeof(*(a)))

CTF_TEST(letter_z) { expect_true(letter('z')); }
CTF_TEST(letter_a) { expect_true(letter('a')); }
CTF_TEST(letter_A) { expect_true(letter('A')); }
CTF_TEST(letter_Z) { expect_true(letter('Z')); }
CTF_TEST(letter_number) { expect_false(letter('1')); }
CTF_TEST(letter_between_lower_and_upper) { expect_false(letter('^')); }

CTF_TEST(uppercase_exact) { expect_int_eq(uppercase('A'), 'A'); }
CTF_TEST(uppercase_lower) { expect_int_eq(uppercase('a'), 'A'); }

CTF_TEST(equal_exact) { expect_true(equal('a', 'a')); }
CTF_TEST(equal_lower) { expect_true(equal('A', 'a')); }
CTF_TEST(equal_upper) { expect_true(equal('a', 'A')); }
CTF_TEST(equal_symbol) { expect_false(equal('~', '^')); }

#define TEST_NODE_MATCHES(name, str, expr, ...)  \
  CTF_TEST(node_matches_##name) {                \
    uint res[] = {__VA_ARGS__};                  \
    uint range[LENGTH(res)];                     \
    expect_true(node_matches(range, str, expr)); \
    expect_array_uint_eq(range, res);            \
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
CTF_TEST(node_matches_space_false) {
  expect_false(node_matches(NULL, "pr", "pr test"));
}
CTF_TEST(node_matches_expr_shorter_than_path) {
  expect_false(node_matches(NULL, "path", "s"));
}

#define TEST_MATCHES(name, str, expr, count, ...)  \
  CTF_TEST(matches_##name) {                       \
    uint res[] = {__VA_ARGS__};                    \
    uint range[LENGTH(res)];                       \
    const char *e[] = {expr};                      \
    expect_true(matches(range, str, e, 1, count)); \
    expect_array_uint_eq(range, res);              \
  }
#define TEST_NOT_MATCHES(name, str, count, ...)            \
  CTF_TEST(matches_##name) {                               \
    const char *e[] = {__VA_ARGS__};                       \
    expect_false(matches(NULL, str, e, LENGTH(e), count)); \
  }

CTF_TEST(matches_space_direct) {
  uint res[] = {0, 1, 9, 10};
  uint range[LENGTH(res)];
  const char *expr[] = {"p", "r"};
  expect_true(matches(range, "projects/real", expr, LENGTH(expr), 2));
  expect_array_uint_eq(range, res);
}
CTF_TEST(matches_space_indirect) {
  uint res[] = {0, 1, 12, 13};
  uint range[LENGTH(res)];
  const char *expr[] = {"p", "r"};
  expect_true(matches(range, "projects/no/real", expr, LENGTH(expr), 2));
  expect_array_uint_eq(range, res);
}
TEST_MATCHES(slash, "projects/real", "p/r", 2, 0, 1, 9, 10)
TEST_MATCHES(middle, "project", "e", 1, 4, 5)
TEST_MATCHES(absolute, "/rp", "/p", 1, 2, 3)
CTF_TEST(matches_absolute_middle) {
  uint res[] = {2, 3, 6, 7};
  uint range[LENGTH(res)];
  const char *e[] = {"/p", "s"};
  expect_true(matches(range, "/rp/test", e, 2, 2));
  expect_array_uint_eq(range, res);
}
CTF_TEST(matches_absolute_space) {
  uint res[] = {0, 1, 6, 7};
  uint range[LENGTH(res)];
  const char *e[] = {"/", "p"};
  expect_true(matches(range, "/test/pro", e, 2, 2));
  expect_array_uint_eq(range, res);
}
TEST_MATCHES(second, "test/project", "p", 1, 5, 6)
TEST_MATCHES(skip_one, "projects/lol/test", "p//t", 3, 0, 1, 11, 11, 13, 14)
TEST_MATCHES(always_skips, "projects/test/test2", "p//t", 3, 0, 1, 12, 12, 14,
             15)
TEST_NOT_MATCHES(first_false, "test", 1, "pr")
TEST_NOT_MATCHES(absolute_false, "/test", 1, "/p")
TEST_NOT_MATCHES(longer_path, "projects/test", 1, "p")
TEST_NOT_MATCHES(space_false, "projects/no/luck", 2, "p", "r")
TEST_NOT_MATCHES(expr_left, "projects/false", 2, "a", "o")
TEST_NOT_MATCHES(absolute_relative, "pr", 1, "/p")
TEST_NOT_MATCHES(absolute_indirect, "/test/pro", 1, "/p")
TEST_NOT_MATCHES(slash_false, "projects/no", 2, "p/r")

CTF_GROUP(match_group) = {
  letter_a,
  letter_A,
  letter_between_lower_and_upper,
  letter_number,
  letter_z,
  letter_Z,
  uppercase_exact,
  uppercase_lower,
  equal_exact,
  equal_lower,
  equal_symbol,
  equal_upper,
  node_matches_case,
  node_matches_empty,
  node_matches_exact,
  node_matches_expr_shorter_than_path,
  node_matches_infix,
  node_matches_prefix,
  node_matches_prefix_repetition,
  node_matches_slash,
  node_matches_space,
  node_matches_space_false,
  node_matches_suffix,
  matches_absolute,
  matches_absolute_false,
  matches_absolute_indirect,
  matches_absolute_middle,
  matches_absolute_relative,
  matches_absolute_space,
  matches_always_skips,
  matches_first_false,
  matches_longer_path,
  matches_expr_left,
  matches_middle,
  matches_second,
  matches_skip_one,
  matches_slash,
  matches_slash_false,
  matches_space_direct,
  matches_space_false,
  matches_space_indirect,
};
