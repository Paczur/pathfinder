#include "score.h"
#include <score/score.c>

CTF_TEST(test_score_depth) {
  stats_t stats1 = STATS_INIT(1);
  stats_t stats2 = STATS_INIT(1);
  stats1.depth = 2;
  stats2.depth = 1;
  expect_uint_gt(score(&stats2, 1), score(&stats1, 1));
}

CTF_TEST(test_score_dotfile) {
  stats_t stats2 = STATS_INIT(1);
  stats_t stats1 = STATS_INIT(1);
  stats1.depth = 1;
  stats2.depth = 1;
  stats1.dotfile[0] = true;
  expect_uint_gt(score(&stats2, 1), score(&stats1, 1));
}

CTF_TEST(test_score_dirname_start) {
  stats_t stats1 = STATS_INIT(1);
  stats_t stats2 = STATS_INIT(1);
  stats1.depth = 1;
  stats2.depth = 1;
  stats1.dirname_start[0] = 1;
  stats1.dirname_end[0] = 1;
  expect_uint_gt(score(&stats2, 1), score(&stats1, 1));
}

CTF_TEST(test_score_dirname_end) {
  stats_t stats2 = STATS_INIT(1);
  stats_t stats1 = STATS_INIT(1);
  stats1.depth = 1;
  stats2.depth = 1;
  stats1.dirname_start[0] = 2;
  stats2.dirname_start[0] = 3;
  stats1.dirname_end[0] = 1;
  stats2.dirname_end[0] = 0;
  expect_uint_gt(score(&stats2, 1), score(&stats1, 1));
}

CTF_TEST(test_score_dirname_priority) {
  stats_t stats1 = STATS_INIT(1);
  stats_t stats2 = STATS_INIT(1);
  stats1.depth = 1;
  stats2.depth = 1;
  stats1.dirname_start[0] = 1;
  stats2.dirname_start[0] = 1;
  stats1.dirname_end[0] = 2;
  stats2.dirname_end[0] = 1;
  expect_uint_gte(score(&stats2, 1), score(&stats1, 1));
}

CTF_TEST(test_score_word_start) {
  stats_t stats1 = STATS_INIT(1);
  stats_t stats2 = STATS_INIT(1);
  stats1.depth = 1;
  stats2.depth = 1;
  stats1.word_start[0] = 1;
  stats1.word_end[0] = 1;
  expect_uint_gt(score(&stats2, 1), score(&stats1, 1));
}

CTF_TEST(test_score_word_end) {
  stats_t stats1 = STATS_INIT(1);
  stats_t stats2 = STATS_INIT(1);
  stats1.depth = 1;
  stats2.depth = 1;
  stats1.word_start[0] = 2;
  stats2.word_start[0] = 3;
  stats1.word_end[0] = 1;
  stats2.word_end[0] = 0;
  expect_uint_gt(score(&stats2, 1), score(&stats1, 1));
}

CTF_TEST(test_score_word_priority) {
  stats_t stats1 = STATS_INIT(1);
  stats_t stats2 = STATS_INIT(1);
  stats1.depth = 1;
  stats2.depth = 1;
  stats1.word_start[0] = 1;
  stats2.word_start[0] = 1;
  stats1.word_end[0] = 1;
  stats2.word_end[0] = 2;
  expect_uint_gte(score(&stats2, 1), score(&stats1, 1));
}

CTF_TEST(test_score_up_case) {
  stats_t stats1 = STATS_INIT(1);
  stats_t stats2 = STATS_INIT(1);
  stats1.depth = 1;
  stats2.depth = 1;
  stats1.up_case[0] = 1;
  expect_uint_gt(score(&stats2, 1), score(&stats1, 1));
}

CTF_TEST(test_score_low_case) {
  stats_t stats1 = STATS_INIT(1);
  stats_t stats2 = STATS_INIT(1);
  stats1.depth = 1;
  stats2.depth = 1;
  stats1.low_case[0] = 1;
  expect_uint_gt(score(&stats2, 1), score(&stats1, 1));
}

CTF_TEST(test_score_length) {
  stats_t stats1 = STATS_INIT(1);
  stats_t stats2 = STATS_INIT(1);
  stats1.depth = 1;
  stats2.depth = 1;
  stats1.dirname_end[0] = 1;
  expect_uint_gt(score(&stats2, 1), score(&stats1, 1));
}

CTF_GROUP(score_group) = {
  test_score_depth,         test_score_dotfile,
  test_score_dirname_start, test_score_dirname_priority,
  test_score_dirname_end,   test_score_word_start,
  test_score_word_end,      test_score_word_priority,
  test_score_up_case,       test_score_low_case,
  test_score_length,
};
