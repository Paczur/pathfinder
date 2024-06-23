#include "score.c"
#include "test.h"

TEST(score, depth) {
  stats_t stats1 = STATS_INIT(1);
  stats_t stats2 = STATS_INIT(1);
  stats1.depth = 2;
  stats2.depth = 1;
  assert_int_bigger(score(&stats2, 1), score(&stats1, 1));
}

TEST(score, dotfile) {
  stats_t stats2 = STATS_INIT(1);
  stats_t stats1 = STATS_INIT(1);
  stats1.depth = 1;
  stats2.depth = 1;
  stats1.dotfile[0] = true;
  assert_int_bigger(score(&stats2, 1), score(&stats1, 1));
}

TEST(score, dirname_start) {
  stats_t stats1 = STATS_INIT(1);
  stats_t stats2 = STATS_INIT(1);
  stats1.depth = 1;
  stats2.depth = 1;
  stats1.dirname_start[0] = 1;
  stats1.dirname_end[0] = 1;
  assert_int_bigger(score(&stats2, 1), score(&stats1, 1));
}

TEST(score, dirname_end) {
  stats_t stats2 = STATS_INIT(1);
  stats_t stats1 = STATS_INIT(1);
  stats1.depth = 1;
  stats2.depth = 1;
  stats1.dirname_start[0] = 2;
  stats2.dirname_start[0] = 3;
  stats1.dirname_end[0] = 1;
  stats2.dirname_end[0] = 0;
  assert_int_bigger(score(&stats2, 1), score(&stats1, 1));
}

TEST(score, dirname_priority) {
  stats_t stats1 = STATS_INIT(1);
  stats_t stats2 = STATS_INIT(1);
  stats1.depth = 1;
  stats2.depth = 1;
  stats1.dirname_start[0] = 1;
  stats2.dirname_start[0] = 1;
  stats1.dirname_end[0] = 2;
  stats2.dirname_end[0] = 1;
  assert_int_bigger_or_equal(score(&stats2, 1), score(&stats1, 1));
}

TEST(score, word_start) {
  stats_t stats1 = STATS_INIT(1);
  stats_t stats2 = STATS_INIT(1);
  stats1.depth = 1;
  stats2.depth = 1;
  stats1.word_start[0] = 1;
  stats1.word_end[0] = 1;
  assert_int_bigger(score(&stats2, 1), score(&stats1, 1));
}

TEST(score, word_end) {
  stats_t stats1 = STATS_INIT(1);
  stats_t stats2 = STATS_INIT(1);
  stats1.depth = 1;
  stats2.depth = 1;
  stats1.word_start[0] = 2;
  stats2.word_start[0] = 3;
  stats1.word_end[0] = 1;
  stats2.word_end[0] = 0;
  assert_int_bigger(score(&stats2, 1), score(&stats1, 1));
}

TEST(score, word_priority) {
  stats_t stats1 = STATS_INIT(1);
  stats_t stats2 = STATS_INIT(1);
  stats1.depth = 1;
  stats2.depth = 1;
  stats1.word_start[0] = 1;
  stats2.word_start[0] = 1;
  stats1.word_end[0] = 1;
  stats2.word_end[0] = 2;
  assert_int_bigger_or_equal(score(&stats2, 1), score(&stats1, 1));
}

TEST(score, up_case) {
  stats_t stats1 = STATS_INIT(1);
  stats_t stats2 = STATS_INIT(1);
  stats1.depth = 1;
  stats2.depth = 1;
  stats1.up_case[0] = 1;
  assert_int_bigger(score(&stats2, 1), score(&stats1, 1));
}

TEST(score, low_case) {
  stats_t stats1 = STATS_INIT(1);
  stats_t stats2 = STATS_INIT(1);
  stats1.depth = 1;
  stats2.depth = 1;
  stats1.low_case[0] = 1;
  assert_int_bigger(score(&stats2, 1), score(&stats1, 1));
}

TEST(score, length) {
  stats_t stats1 = STATS_INIT(1);
  stats_t stats2 = STATS_INIT(1);
  stats1.depth = 1;
  stats2.depth = 1;
  stats1.dirname_end[0] = 1;
  assert_int_bigger(score(&stats2, 1), score(&stats1, 1));
}

int main(void) {
  const struct CMUnitTest tests[] = {
    ADD(score, depth),         ADD(score, dotfile),
    ADD(score, dirname_start), ADD(score, dirname_priority),
    ADD(score, dirname_end),   ADD(score, word_start),
    ADD(score, word_end),      ADD(score, word_priority),
    ADD(score, up_case),       ADD(score, low_case),
    ADD(score, length)};
  return cmocka_run_group_tests(tests, NULL, NULL);
}
