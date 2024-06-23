#ifndef H_SCORE
#define H_SCORE

#include "types.h"
#include "stat.h"
#include <limits.h>

#define SCORE_BASE UINT_MAX
#define SCORE_DEPTH 10000
#define SCORE_DOTFILE 110
#define SCORE_DIRNAME_END 100
#define SCORE_DIRNAME_START 40
#define SCORE_WORD_END 15
#define SCORE_WORD_START 10
#define SCORE_BAD_CASE 5
#define SCORE_LENGTH 2

#define SCORE_LOSS 4

uint score(const stats_t *stats, uint count);

#endif
