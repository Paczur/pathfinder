#include "score.c"
#include "test.h"

TEST(score, simple) {
  stats_t stats = {.depth = 2,
                   .count = 2,
                   .dirname_start = &(uchar[]){0, 1},
                   .dirname_end = &(uchar[]){1, 0},
                   .word_start = &(uchar[]){0, 1},
                   .word_end = &(uchar[]){1, 0},
                   .bad_case = &(uchar[]){1, 2}};
  int ans = score(&stats);
  assert_int_equal(ans, 83750);
}

TEST(score, one) {
  stats_t stats = {.depth = 1,
                   .count = 1,
                   .dirname_start = &(uchar[]){0},
                   .dirname_end = &(uchar[]){1},
                   .word_start = &(uchar[]){0},
                   .word_end = &(uchar[]){1},
                   .bad_case = &(uchar[]){1}};
  int ans = score(&stats);
  assert_int_equal(ans, 91000);
}

int main(void) {
  const struct CMUnitTest tests[] = {
    ADD(score, simple),
    ADD(score, one),
  };
  return cmocka_run_group_tests(tests, NULL, NULL);
}
