#ifndef H_SCORE
#define H_SCORE

#include "types.h"

#define SCORE_BASE 10 * SCORE_PRECISION
#define SCORE_PRECISION 10000
#define SCORE_DEPTH 1
#define SCORE_DIRNAME_START 0.8
#define SCORE_DIRNAME_END 0.6
#define SCORE_WORD_START 0.4
#define SCORE_WORD_END 0.2
#define SCORE_BAD_CASE 0.1

#define SCORE_LOSS 4

uint score(const stats_t *stats);

#endif
