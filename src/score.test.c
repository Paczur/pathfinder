#include "score.c"
#include "test.h"

TEST(score, simple) {
  stats_t stats = {.depth = 2,
                   .dirname_start = &(uint[]){0, 1},
                   .dirname_end = &(uint[]){1, 0},
                   .word_start = &(uint[]){0, 1},
                   .word_end = &(uint[]){1, 0},
                   .bad_case = &(uint[]){1, 2}};
  int ans = score(&stats, 2);
  assert_int_equal(ans, 83750);
}

TEST(score, one) {
  stats_t stats = {.depth = 1,
                   .dirname_start = &(uint[]){0},
                   .dirname_end = &(uint[]){1},
                   .word_start = &(uint[]){0},
                   .word_end = &(uint[]){1},
                   .bad_case = &(uint[]){1}};
  int ans = score(&stats, 1);
  assert_int_equal(ans, 91000);
}

int main(void) {
  const struct CMUnitTest tests[] = {
    ADD(score, simple),
    ADD(score, one),
  };
  return cmocka_run_group_tests(tests, NULL, NULL);
}
