#include "match.h"
#include <match/match.h>

#define LENGTH(a) (sizeof(a) / sizeof(*(a)))

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
