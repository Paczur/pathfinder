#ifndef H_MATCH
#define H_MATCH

#include "types.h"

bool matches(uint *ranges, const char *restrict str,
             const char *restrict const *expr, uint len, uint count);

#endif
