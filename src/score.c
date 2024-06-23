#include "score.h"
#include <stdio.h>

#define SCORE_INIT(field, score)                                              \
  stats->field[0] * score / ((count == 1) ? 1 : (SCORE_LOSS * (count - 1))) + \
    ((count == 1) ? 0 : stats->field[count - 1] * score)

#define SCORE_ADD(field, score, i) \
  stats->field[i] * score / (SCORE_LOSS * (count - 1 - i))
#define SCORE_MIN_ADD(field1, field2, score1, score2, i) \
  ((stats->field1[i] <= stats->field2[i])                \
     ? ((stats->field1[i] + 1) * score1)                 \
     : ((stats->field2[i] + 1) * score2)) /              \
    (SCORE_LOSS * (count - 1 - i));

uint score(const stats_t *stats, uint count) {
  uint depth = (stats->depth - count) * SCORE_DEPTH;
  uint dirname = (stats->dirname_start[0] <= stats->dirname_end[0])
                   ? ((stats->dirname_start[0] + 1) * SCORE_DIRNAME_START)
                   : ((stats->dirname_end[0] + 1) * SCORE_DIRNAME_END);
  uint word = (stats->word_start[0] <= stats->word_end[0])
                ? ((stats->word_start[0] + 1) * SCORE_DIRNAME_START)
                : ((stats->word_end[0] + 1) * SCORE_DIRNAME_END);
  uint length =
    (stats->dirname_start[0] + stats->dirname_end[0]) * SCORE_LENGTH;
  if(count != 1) {
    dirname /= SCORE_LOSS * (count - 1);
    word /= SCORE_LOSS * (count - 1);
    length /= SCORE_LOSS * (count - 1);
  } else {
    dirname +=
      (stats->dirname_start[count - 1] <= stats->dirname_end[count - 1])
        ? ((stats->dirname_start[count - 1] + 1) * SCORE_DIRNAME_START)
        : ((stats->dirname_end[count - 1] + 1) * SCORE_DIRNAME_END);
    word += (stats->word_start[count - 1] <= stats->word_end[count - 1])
              ? ((stats->word_start[count - 1] + 1) * SCORE_DIRNAME_START)
              : ((stats->word_end[count - 1] + 1) * SCORE_DIRNAME_END);
    length +=
      (stats->dirname_start[count - 1] + stats->dirname_end[count - 1]) *
      SCORE_LENGTH;
  }
  uint bad_case = SCORE_INIT(bad_case, SCORE_BAD_CASE);
  if(count >= 2) {
    for(size_t i = count - 2; i > 0; i--) {
      dirname += SCORE_MIN_ADD(dirname_start, dirname_end, SCORE_DIRNAME_START,
                               SCORE_DIRNAME_END, i);
      word += SCORE_MIN_ADD(word_start, word_end, SCORE_DIRNAME_START,
                            SCORE_DIRNAME_END, i);
      bad_case += SCORE_ADD(bad_case, SCORE_BAD_CASE, i);
      length +=
        (stats->dirname_start[count - 1] + stats->dirname_end[count - 1]) *
        SCORE_LENGTH;
    }
  }
  return SCORE_BASE - depth - dirname - word - bad_case - length;
}
