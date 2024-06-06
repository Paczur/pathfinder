#include "parse.h"

uchar node_count(const char *expr) {
  uchar counter = 1;
  if(!expr[1]) return expr[0] != '/';
  for(size_t i = 1; expr[i + 1]; i++) {
    if(expr[i] == '/' || expr[i] == ' ') counter++;
  }
  return counter;
}
