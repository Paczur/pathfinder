#include "match.h"
#include "res.h"
#include "score.h"
#include "stats.h"
#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

#define HELP_MSG                                                             \
  "Usage: pf [OPTION]... EXPR...\n"                                          \
  "Find path(s) best matching EXPR using substring matches.\n"               \
  "\n"                                                                       \
  "General options:\n"                                                       \
  "-h, --help          Show help\n"                                          \
  "-m, --max-matches   Number of matches to print (default 9)\n"             \
  "-v, --verbose       Print errors to stderr\n"                             \
  "-i, --non-interactive Print all paths immediatly, don't wait for user "   \
  "choice\n"                                                                 \
  "\n"                                                                       \
  "Search Options:\n"                                                        \
  "--ignore-dotfiles   Skip directories and symlinks beginning with \".\"\n" \
  "-M, --max-depth     Max depth to search down the tree (default 5)\n"

uint *ranges;
struct stat sstat;
stats_t st;
bool unlimited;
bool interactive = true;
bool interactive;
bool verbose;
bool ignore_dotfiles;
size_t max_depth = 5;
resl_t list;
resa_t arr = {.size = 0, .limit = 9};

static void cleanup(void) {
#ifndef NDEBUG
  if(unlimited) {
    resl_free(&list);
  } else {
    resa_free(&arr);
  }
  free(ranges);
#endif
}

static uint node_count(const char *const *expr, uint len) {
  uint res = len;
  for(size_t i = 0; i < len; i++) {
    for(size_t j = 0; expr[i][j]; j++) {
      if(expr[i][j] == '/') res++;
    }
  }
  return res;
}

static uint handleinf(const char *str, const char *const *expr, uint len,
                      uint count) {
  resn_t *new;
  stats_t *s;
  if(matches(ranges, str, expr, len, count)) {
    new = malloc(sizeof(resn_t));
#ifndef NDEBUG
    s = &new->stats;
#else
    s = &st;
#endif
    resn_alloc(new, strlen(str) + 1, count);
    strcpy(new->path, str);
    stats(s, ranges, count * 2, expr, str);
    new->score = score(s, count);
    resl_add(&list, new);
    return new->score;
  }
  return 0;
}

static uint handle(const char *str, const char *const *expr, uint len,
                   uint count) {
  resv_t new;
  stats_t *s;
  if(matches(ranges, str, expr, len, count)) {
#ifndef NDEBUG
    s = &new.stats;
#else
    s = &st;
#endif
    resv_alloc(&new, strlen(str) + 1, count);
    strcpy(new.path, str);
    stats(s, ranges, count * 2, expr, str);
    new.score = score(s, count);
    if(!resa_add(&arr, &new)) {
      resv_free(&new);
    } else {
      return new.score;
    }
  }
  return 0;
}

static void iter_paths(const char *const *expr, uint len, uint count) {
  char path[512] = ".";
  struct dirent *de;
  uint *null_stack;
  uint new_null = 1;
  DIR **dr_stack = malloc((max_depth + 1) * sizeof(DIR *));
  uint stack_i = 0;
  dr_stack[0] = opendir(path);
  if(!dr_stack[0]) {
    if(verbose) fprintf(stderr, "Couldn't open directory: %s\n", path);
    free(dr_stack);
    return;
  }
  null_stack = malloc((max_depth + 1) * sizeof(uint));

  null_stack[0] = 1;
  while(1) {
    de = readdir(dr_stack[stack_i]);
    if(!de) {
      closedir(dr_stack[stack_i]);
      if(stack_i == 0) break;
      stack_i--;
      continue;
    }
    if((de->d_type != DT_LNK && de->d_type != DT_DIR) ||
       (de->d_name[0] == '.' &&
        (ignore_dotfiles || de->d_name[1] == 0 ||
         (de->d_name[1] == '.' && de->d_name[2] == 0)))) {
      continue;
    }
    path[null_stack[stack_i]] = '/';
    new_null = stpcpy(path + null_stack[stack_i] + 1, de->d_name) - path;

    if((de->d_type == DT_DIR ||
        (!stat(path, &sstat) && S_ISDIR(sstat.st_mode))) &&
       SCORE_BASE == ((unlimited) ? handleinf(path + 2, expr, len, count)
                                  : handle(path + 2, expr, len, count))) {
      for(size_t i = 0; i <= stack_i; i++) {
        closedir(dr_stack[i]);
      }
      break;
    }
    if(de->d_type == DT_DIR && stack_i < max_depth) {
      stack_i++;
      dr_stack[stack_i] = opendir(path);
      if(!dr_stack[stack_i]) {
        stack_i--;
      } else {
        null_stack[stack_i] = new_null;
      }
    }
  }

  free(dr_stack);
  free(null_stack);
}

int main(int argc, const char *const argv[]) {
  uint nc;
  size_t n;
  long long t;
  uint off = 1;
  if(argc < 2) goto error;
  for(; off < (uint)argc; off++) {
    if(argv[off][0] != '-') break;
    if(!strcmp(argv[off], "-h") || !strcmp(argv[off], "--help")) {
      puts(HELP_MSG);
      goto cleanup;
    } else if(!strcmp(argv[off], "-m") || !strcmp(argv[off], "--max-matches")) {
      if(off + 1 >= (uint)argc || !sscanf(argv[off + 1], "%lld", &t)) {
        goto error;
      }
      if(t < 1) {
        unlimited = true;
      } else {
        arr.limit = (uint)t;
      }
      off++;
    } else if(!strcmp(argv[off], "-v") || !strcmp(argv[off], "--verbose")) {
      verbose = true;
    } else if(!strcmp(argv[off], "--ignore-dotfiles")) {
      ignore_dotfiles = true;
    } else if(!strcmp(argv[off], "-M") || !strcmp(argv[off], "--max-depth")) {
      if(off + 1 >= (uint)argc || !sscanf(argv[off + 1], "%lld", &t) || t < 0) {
        goto error;
      }
      max_depth = t;
      off++;
    } else if(!strcmp(argv[off], "-i") ||
              !strcmp(argv[off], "--non-interactive")) {
      interactive = false;
    }
  }
  if(off >= (uint)argc) {
    puts(HELP_MSG);
    goto cleanup;
  }
  nc = node_count(argv + off, argc - off);

  ranges = malloc(nc * 2 * sizeof(uint));
  if(!unlimited) resa_alloc(&arr);
#ifdef NDEBUG
  stats_alloc(&st, nc);
#endif

  iter_paths(argv + off, argc - off, nc);

#ifdef NDEBUG
  stats_free(&st);
#endif

#ifndef NDEBUG
  if(unlimited) {
    resl_print(&list, nc);
  } else {
    resa_print(&arr, nc);
  }
  puts("");
#endif

  if(interactive) {
    if(unlimited) {
      resl_numbered_path_print(&list);
    } else {
      resa_numbered_path_print(&arr);
    }
    fputs("Select appropriate path:\n", stderr);
    if(!scanf("%lu", &n)) goto error;
    if(unlimited) {
      resl_i_path_print(&list, n);
    } else {
      resa_i_path_print(&arr, n);
    }
  } else {
    if(unlimited) {
      resl_path_print(&list);
    } else {
      resa_path_print(&arr);
    }
  }

cleanup:
  cleanup();
  return 0;

error:
  puts(HELP_MSG);
  cleanup();
  return 1;
}
