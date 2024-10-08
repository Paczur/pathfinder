#ifndef H_SCORE
#define H_SCORE

#include "types.h"
#include "stats.h"
#include <limits.h>

#define SCORE_BASE UINT_MAX
#define SCORE_DIRNAME_END 260
#define SCORE_DIRNAME_START 160
#define SCORE_LOW_CASE 100
#define SCORE_DEPTH 90
#define SCORE_DOTFILE 70
#define SCORE_WORD_END 60
#define SCORE_WORD_START 30
#define SCORE_UP_CASE 20
#define SCORE_LENGTH 5
#define SCORE_PATH_LEN 1

#define SCORE_LOSS 2

uint score(const stats_t *stats, uint count);

#endif
