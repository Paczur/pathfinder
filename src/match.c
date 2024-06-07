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

static bool node_matches(const char *expr, const char *str, uchar *range) {
  assert(expr);
  assert(str);
  assert(expr[0]);
  assert(str[0]);
  size_t expr_i = 0;
  size_t i = 0;
  for(; str[i] && str[i] != '/'; i++) {
    if(equal(str[i], expr[expr_i])) {
      expr_i++;
      if(!expr[expr_i] || expr[expr_i] == ' ' || expr[expr_i] == '/') {
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

// TODO: requires refactor
bool matches(const char *expr, const char *str, uchar *ranges) {
  assert(expr);
  assert(str);
  assert(expr[0]);
  assert(str[0]);
  bool match;
  size_t ranges_i = 0;
  bool space = true;
  size_t str_i = 0;
  size_t i = 0;
  if(expr[0] == '/') {
    if(str[0] != '/') return false;
    str_i++;
    i++;
    space = false;
    if(expr[1] == ' ') {
      space = true;
      i++;
    }
  }
  match = node_matches(expr + i, str + str_i, ranges);
  if(space && !match) {
    for(; str[str_i]; str_i++) {
      if(str[str_i - 1] != '/') continue;
      match = node_matches(expr + i, str + str_i, ranges);
      if(match) break;
    }
  }
  if(!match) return false;
  if(ranges) {
    ranges[0] += str_i;
    ranges[1] += str_i;
    ranges_i += 2;
    str_i = ranges[1];
  }
  for(; expr[i]; i++) {
    if(expr[i] != ' ' && expr[i] != '/') continue;
    while(str[str_i] && str[str_i] != '/') str_i++;
    if(!str[str_i] || !str[str_i + 1]) return false;
    space = expr[i] == ' ';
    i++;
    str_i++;
    match = node_matches(expr + i, str + str_i, ranges + ranges_i);
    if(!space && !match) return false;
    if(match) {
      if(ranges) {
        ranges[ranges_i] += str_i;
        ranges[ranges_i + 1] += str_i;
        str_i = ranges[ranges_i + 1];
        ranges_i += 2;
      }
      continue;
    }
    for(; str[str_i]; str_i++) {
      if(str[str_i - 1] != '/') continue;
      match = node_matches(expr + i, str + str_i, ranges + ranges_i);
      if(match) {
        if(ranges) {
          ranges[ranges_i] += str_i;
          ranges[ranges_i + 1] += str_i;
          str_i = ranges[ranges_i + 1];
          ranges_i += 2;
        }
        break;
      }
    }
  }
  return match;
}
