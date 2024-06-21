#ifndef H_MATCH
#define H_MATCH

#include "types.h"

bool matches(uint *ranges, const char *str, const char *const *expr,
             uint len);

#endif
