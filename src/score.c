#include "score.h"
#include <stdio.h>

#define SCORE_INIT(field)                                                   \
  uint field =                                                              \
    stats->field[0] * SCORE_PRECISION / (SCORE_LOSS * (stats->count - 1)) + \
    stats->field[stats->count - 1] * SCORE_PRECISION
#define SCORE_ADD(field, i) \
  field +=                  \
    stats->field[i] * SCORE_PRECISION / (SCORE_LOSS * (stats->count - 1 - i))

uint score(const stats_t *stats) {
  uint depth = (stats->depth - stats->count) * SCORE_PRECISION;
  SCORE_INIT(dirname_start);
  SCORE_INIT(dirname_end);
  SCORE_INIT(word_start);
  SCORE_INIT(word_end);
  SCORE_INIT(bad_case);
  for(size_t i = stats->count - 2; i > 0; i--) {
    SCORE_ADD(dirname_start, i);
    SCORE_ADD(dirname_end, i);
    SCORE_ADD(word_start, i);
    SCORE_ADD(word_end, i);
    SCORE_ADD(bad_case, i);
  }
  depth *= SCORE_DEPTH;
  dirname_start *= SCORE_DIRNAME_START;
  dirname_end *= SCORE_DIRNAME_END;
  word_start *= SCORE_WORD_START;
  word_end *= SCORE_WORD_END;
  bad_case *= SCORE_BAD_CASE;
  return SCORE_BASE - depth - dirname_start - dirname_end - word_start -
         word_end - bad_case;
}
