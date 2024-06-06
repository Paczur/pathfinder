#include "match.h"
#include <assert.h>
#include <stdlib.h>

static bool letter(char c) {
  return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z');
}

static char uppercase(char c) {
  assert(letter(c));
  return (c >= 'a') ? c - 'a' + 'A' : c;
}

static bool equal(char c1, char c2) {
  return (letter(c1) ? uppercase(c1) : c1) == (letter(c2) ? uppercase(c2) : c2);
}

static bool node_matches(const char *expr, const char *str) {
  size_t expr_i = 0;
  size_t i = 0;
  for(; str[i] && str[i] != '/'; i++) {
    if(equal(str[i], expr[expr_i])) {
      expr_i++;
      if(!expr[expr_i] || expr[expr_i] == ' ' || expr[expr_i] == '/')
        return true;
    } else if(expr_i > 0) {
      expr_i = equal(str[i], expr[0]) ? 1 : 0;
    }
  }
  return expr[expr_i] == str[i] &&
         (!expr[expr_i + 1] || expr[expr_i + 1] == ' ' ||
          expr[expr_i + 1] == '/');
}

uchar node_count(const char *expr) {
  uchar counter = 1;
  if(!expr[1]) return expr[0] != '/';
  for(size_t i = 1; expr[i + 1]; i++) {
    if(expr[i] == '/' || expr[i] == ' ') counter++;
  }
  return counter;
}

bool matches(const char *expr, const char *str, uchar *node_is) {
  bool match = false;
  bool space = expr[0] != '/';
  size_t str_i = 0;
  match = node_matches(expr, str);
  if(!match) {
    for(; str[str_i]; str_i++) {
      if(str[str_i - 1] != '/') continue;
      match = node_matches(expr, str + str_i);
      if(match) break;
    }
  }
  for(size_t i = 1; expr[i]; i++) {
    if(expr[i] != ' ' && expr[i] != '/') continue;
    while(str[str_i] && str[str_i] != '/') str_i++;
    if(!str[str_i] || !str[str_i + 1]) return false;
    space = expr[i] == ' ';
    i++;
    str_i++;
    match = node_matches(expr + i, str + str_i);
    if(!space && !match) return false;
    if(!space) continue;
    for(; str[str_i]; str_i++) {
      if(str[str_i - 1] != '/') continue;
      match = node_matches(expr + i, str + str_i);
      if(match) break;
    }
  }
  return !space || match;
}
