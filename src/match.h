#ifndef H_MATCH
#define H_MATCH

#include "types.h"

bool matches(uchar *ranges, const char *str, const char *const *expr,
             uchar len);

#endif
