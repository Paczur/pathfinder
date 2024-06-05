#include <stdbool.h>
#include <stddef.h>
#include <assert.h>

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

bool matches(const char *expr, const char *str) {
  size_t str_i = 0;
  if(!node_matches(expr, str)) return false;
  for(size_t i = 0; expr[i]; i++) {
    if(expr[i] == ' ' || expr[i] == '/') {
      while(str[str_i] && str[str_i] != '/') str_i++;
      if(!str[str_i] || !str[str_i + 1]) return false;
      i++;
      str_i++;
      printf("%s %s\n", expr + i, str + str_i);
      if(!node_matches(expr + i, str + str_i)) return false;
    }
  }
  return true;
}
