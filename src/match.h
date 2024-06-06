#ifndef H_MATCH
#define H_MATCH

#include "types.h"

uchar node_count(const char *expr);
bool matches(const char *expr, const char *str, uchar *node_is);

#endif
