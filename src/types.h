#ifndef H_UTIL
#define H_UTIL

#include <stddef.h>
#include <stdbool.h>

#define puref __attribute__((pure))
#define constf __attribute__((const))
#define readonlya(x) __attribute__((access(read_only, x)))
#define rda(x) readonlya(x)
#define wra(x) __attribute__((access(write_only, x)))
#define nonea(x) __attribute__((access(none, x)))

typedef unsigned char uchar;
typedef unsigned int uint;

#endif
