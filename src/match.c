#include "match.h"
#include <assert.h>
#include <stdlib.h>
#include <stdio.h>

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

static bool node_matches(uint *range, const char *str, const char *expr) {
  assert(expr);
  assert(str);
  assert(str[0]);
  if(!expr[0] || expr[0] == '/') {
    size_t len = 0;
    while(str[len] && str[len] != '/') len++;
    if(range) {
      range[0] = len - 1;
      range[1] = len - 1;
    }
    return true;
  }
  size_t expr_i = 0;
  size_t i = 0;
  for(; str[i] && str[i] != '/'; i++) {
    if(equal(str[i], expr[expr_i])) {
      expr_i++;
      if(!expr[expr_i] || expr[expr_i] == '/') {
        if(range) {
          range[1] = i + 1;
          range[0] = range[1] - expr_i;
        }
        return true;
      }
    } else if(expr_i > 0) {
      expr_i = equal(str[i], expr[0]) ? 1 : 0;
    }
  }
  return false;
}

bool matches(uint *ranges, const char *str, const char *const *expr, uint len) {
  assert(expr);
  assert(str);
  assert(expr[0]);
  assert(str[0]);
  size_t node_i = 0;
  size_t expr_i = 0;
  size_t str_i = 0;
  size_t ranges_i = 0;
  bool allow_skip = true;
  bool match = false;

  // handling of absolute paths
  if(expr[node_i][expr_i] == '/') {
    if(str[str_i] != '/') return false;
    expr_i++;
    str_i++;
    if(!expr[node_i][expr_i]) {
      allow_skip = true;
      node_i++;
      expr_i = 0;
      if(ranges) {
        ranges[ranges_i] = 0;
        ranges[ranges_i + 1] = 1;
        ranges_i += 2;
      }
    } else {
      allow_skip = false;
    }
  }

  while(str[str_i]) {
    if(!expr[node_i][expr_i]) {
      if(node_i == (size_t)len - 1)
        return false;  // pattern finnished but string didn't
      node_i++;
      allow_skip = true;
      expr_i = 0;
    }
    if((match = node_matches((ranges) ? ranges + ranges_i : NULL, str + str_i,
                             expr[node_i] + expr_i))) {
      if(ranges) {
        ranges[ranges_i] += str_i;
        ranges[ranges_i + 1] += str_i;
        expr_i += ranges[ranges_i + 1] - ranges[ranges_i];
        if(expr[node_i][expr_i] == '/') expr_i++;
        str_i = ranges[ranges_i + 1];
        ranges_i += 2;
      } else {
        while(expr[node_i][expr_i] && expr[node_i][expr_i] != '/') expr_i++;
        if(expr[node_i][expr_i]) expr_i++;
      }
      allow_skip = false;
    } else if(!allow_skip) {  // didn't match immediately
      break;
    }

    // go to the next path element
    while(str[str_i] && str[str_i] != '/') str_i++;
    if(str[str_i]) str_i++;
  }
  return match;
}
