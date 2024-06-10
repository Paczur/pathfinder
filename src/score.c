#include "score.h"
#include <stdio.h>

#define SCORE_INIT(field)                                                   \
  unsigned int field =                                                      \
    stats->field[0] * SCORE_PRECISION / (SCORE_LOSS * (stats->count - 1)) + \
    stats->field[stats->count - 1] * SCORE_PRECISION
#define SCORE_ADD(field, i) \
  field +=                  \
    stats->field[i] * SCORE_PRECISION / (SCORE_LOSS * (stats->count - 1 - i))

int score(const stats_t *stats) {
  unsigned depth = stats->depth * SCORE_PRECISION;
  SCORE_INIT(dirname_start);
  SCORE_INIT(dirname_end);
  SCORE_INIT(word_start);
  SCORE_INIT(word_end);
  SCORE_INIT(good_case);
  for(size_t i = stats->count - 2; i > 0; i--) {
    SCORE_ADD(dirname_start, i);
    SCORE_ADD(dirname_end, i);
    SCORE_ADD(word_start, i);
    SCORE_ADD(word_end, i);
    SCORE_ADD(good_case, i);
  }
  depth *= SCORE_DEPTH;
  dirname_start *= SCORE_DIRNAME_START;
  dirname_end *= SCORE_DIRNAME_END;
  word_start *= SCORE_WORD_START;
  word_end *= SCORE_WORD_END;
  good_case *= SCORE_GOOD_CASE;
  return 10 * SCORE_PRECISION - depth - dirname_start - dirname_end -
         word_start - word_end + good_case;
}
