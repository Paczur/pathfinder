#include "score.h"
#include <stdio.h>

PURE static uint score_depth(const stats_t *stats, uint count) {
  return (stats->depth - count) * SCORE_DEPTH;
}

PURE static uint score_dirname(const stats_t *const stats, const uint i) {
  const uint dirname_start_raw = stats->dirname_start[i];
  const uint dirname_start =
    dirname_start_raw - (dirname_start_raw > 0 && stats->dotfile[i]);
  const uint dirname_end = stats->dirname_end[i];
  return (dirname_start <= dirname_end) ? (dirname_start * SCORE_DIRNAME_START)
                                        : (dirname_end * SCORE_DIRNAME_END);
}

PURE static uint score_word(const stats_t *const stats, const uint i) {
  const uint word_start = stats->word_start[i];
  const uint word_end = stats->word_end[i];
  return (word_start <= word_end) ? (word_start * SCORE_WORD_START)
                                  : (word_end * SCORE_WORD_END);
}

PURE static uint score_length(const stats_t *const stats, const uint i) {
  return (stats->dirname_start[i] + stats->dirname_end[i]) * SCORE_LENGTH;
}

PURE static uint score_case(const stats_t *const stats, const uint i) {
  return stats->up_case[i] * SCORE_UP_CASE +
         stats->low_case[i] * SCORE_LOW_CASE;
}

PURE static uint score_dotfile(const stats_t *const stats, const uint i) {
  return stats->dotfile[i] * SCORE_DOTFILE;
}

PURE uint score(const stats_t *const stats, const uint count) {
  uint dirname = score_dirname(stats, 0);
  uint word = score_word(stats, 0);
  uint length = score_length(stats, 0);
  uint wrong_case = score_case(stats, 0);
  uint dotfile = score_dotfile(stats, 0);

  if(count > 1) {
    dirname /= SCORE_LOSS * (count - 1);
    word /= SCORE_LOSS * (count - 1);
    length /= SCORE_LOSS * (count - 1);
    wrong_case /= SCORE_LOSS * (count - 1);
    dotfile /= SCORE_LOSS * (count - 1);

    dirname += score_dirname(stats, count - 1);
    word += score_word(stats, count - 1);
    length += score_length(stats, count - 1);
    wrong_case += score_case(stats, count - 1);
    dotfile += score_case(stats, count - 1);

    for(uint i = 1, j = count - 2; i < count - 1; i++, j--) {
      dirname += score_dirname(stats, i) / (SCORE_LOSS * j);
      word += score_word(stats, i) / (SCORE_LOSS * j);
      length += score_length(stats, i) / (SCORE_LOSS * j);
      wrong_case += score_case(stats, i) / (SCORE_LOSS * j);
      dotfile += score_dotfile(stats, i) / (SCORE_LOSS * j);
    }
  }
  return SCORE_BASE - score_depth(stats, count) - dirname - word - wrong_case -
         length - dotfile;
}
