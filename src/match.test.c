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

TEST(node_matches, prefix) { assert_true(node_matches("pr", "pre")); }
TEST(node_matches, suffix) { assert_true(node_matches("pr", "epr")); }
TEST(node_matches, infix) { assert_true(node_matches("pr", "epre")); }
TEST(node_matches, exact) { assert_true(node_matches("pr", "pr")); }
TEST(node_matches, prefix_repetition) {
  assert_true(node_matches("pr", "ppr"));
}
TEST(node_matches, case) { assert_true(node_matches("PR", "pr")); }
TEST(node_matches, slash) { assert_true(node_matches("pr/test", "pr")); }
TEST(node_matches, space) { assert_true(node_matches("pr test2", "pr")); }
TEST(node_matches, expr_shorter_than_path) {
  assert_false(node_matches("s", "path"));
}

TEST(matches, space_direct) { assert_true(matches("p r", "projects/real")); }
TEST(matches, space_indirect) {
  assert_true(matches("p r", "projects/no/real"));
}
TEST(matches, space_false) { assert_false(matches("p r", "projects/no/luck")); }
TEST(matches, slash) { assert_true(matches("p/r", "projects/real")); }
TEST(matches, slash_false) { assert_false(matches("p/r", "projects/no")); }

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
    ADD(matches, slash_false),
  };
  return cmocka_run_group_tests(tests, NULL, NULL);
}
