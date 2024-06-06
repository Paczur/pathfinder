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

TEST(node_matches, prefix) { assert_int_equal(node_matches("pr", "pre"), 0); }
TEST(node_matches, suffix) { assert_int_equal(node_matches("pr", "epr"), 1); }
TEST(node_matches, infix) { assert_int_equal(node_matches("pr", "epre"), 1); }
TEST(node_matches, exact) { assert_int_equal(node_matches("pr", "pr"), 0); }
TEST(node_matches, prefix_repetition) {
  assert_int_equal(node_matches("pr", "ppr"), 1);
}
TEST(node_matches, case) { assert_int_equal(node_matches("PR", "pr"), 0); }
TEST(node_matches, slash) {
  assert_int_equal(node_matches("pr/test", "pr"), 0);
}
TEST(node_matches, space) {
  assert_int_equal(node_matches("pr test2", "pr"), 0);
}
TEST(node_matches, expr_shorter_than_path) {
  assert_int_equal(node_matches("s", "path"), (uchar)-1);
}

TEST(node_count, relative_space) { assert_int_equal(node_count("p r"), 2); }
TEST(node_count, relative_slash) { assert_int_equal(node_count("p/r"), 2); }
TEST(node_count, absolute) { assert_int_equal(node_count("/p"), 1); }
TEST(node_count, root) { assert_int_equal(node_count("/"), 0); }
TEST(node_count, suffix_slash) { assert_int_equal(node_count("p/"), 1); }

SETUP(matches) {
  *state = malloc(2 * sizeof(uchar));
  return 0;
}
TEST(matches, space_direct) {
  uchar *ranges = *state;
  assert_true(matches("p r", "projects/real", ranges));
  assert_true(ranges[0] == 0);
  assert_true(ranges[1] == 9);
}
TEST(matches, space_indirect) {
  uchar *ranges = *state;
  assert_true(matches("p r", "projects/no/real", ranges));
  assert_true(ranges[0] == 0);
  assert_true(ranges[1] == 12);
}
TEST(matches, space_false) {
  assert_false(matches("p r", "projects/no/luck", NULL));
}
TEST(matches, slash) {
  uchar *ranges = *state;
  assert_true(matches("p/r", "projects/real", ranges));
  assert_true(ranges[0] == 0);
  assert_true(ranges[1] == 9);
}
TEST(matches, first_false) { assert_false(matches("pr", "test", NULL)); }
TEST(matches, slash_false) {
  assert_false(matches("p/r", "projects/no", NULL));
}
TEST(matches, middle) {
  uchar *ranges = *state;
  assert_true(matches("e", "project/test", NULL));
  assert_true(ranges[0] == 4);
}
TEST(matches, absolute) {
  uchar *ranges = *state;
  assert_true(matches("/p", "/rp", NULL));
  assert_true(ranges[0] == 2);
}
TEST(matches, absolute_middle) {
  uchar *ranges = *state;
  assert_true(matches("/p s", "/rp/test", NULL));
  assert_true(ranges[0] == 2);
  assert_true(ranges[1] == 6);
}
TEST(matches, absolute_false) { assert_false(matches("/p", "/test", NULL)); }
TEST(matches, second) {
  uchar *ranges = *state;
  assert_true(matches("p", "test/project", NULL));
  assert_true(ranges[0] == 5);
}
TEST(matches, absolute_indirect) {
  uchar *ranges = *state;
  assert_true(matches("p", "test/lol/pro", NULL));
  assert_true(ranges[0] == 9);
}
TEARDOWN(matches) {
  free(*state);
  return 0;
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
    ADD(node_count, relative_space),
    ADD(node_count, relative_slash),
    ADD(node_count, absolute),
    ADD(node_count, root),
    ADD(node_count, suffix_slash),
    ADD(node_matches, suffix),
    ADD(node_matches, prefix),
    ADD(node_matches, infix),
    ADD(node_matches, exact),
    ADD(node_matches, prefix_repetition),
    ADD(node_matches, case),
    ADD(node_matches, slash),
    ADD(node_matches, space),
    ADD(node_matches, expr_shorter_than_path),
  };
  const struct CMUnitTest matches_tests[] = {
    ADD(matches, space_direct),      ADD(matches, space_indirect),
    ADD(matches, space_false),       ADD(matches, slash),
    ADD(matches, first_false),       ADD(matches, slash_false),
    ADD(matches, absolute),          ADD(matches, second),
    ADD(matches, absolute_indirect), ADD(matches, middle),
    ADD(matches, absolute_middle),   ADD(matches, absolute_false),
  };
  return cmocka_run_group_tests(tests, NULL, NULL) ||
         cmocka_run_group_tests(matches_tests, matches_test_setup,
                                matches_test_teardown);
}
