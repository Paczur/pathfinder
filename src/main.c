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
  "-M, --max-depth     Max depth to search down the tree\n"

uint *ranges;
struct stat sstat;
stats_t st;
bool unlimited;
bool interactive = true;
bool interactive;
bool verbose;
bool ignore_dotfiles;
size_t max_depth = 10;
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

static void rec_paths(const char *const *expr, uint len, uint count,
                      uint (*f)(const char *, const char *const *, uint, uint),
                      char *path, size_t iter) {
  struct dirent *de;
  DIR *dr = opendir(path);
  size_t null;
  if(!dr) {
    if(verbose) fprintf(stderr, "Couldn't open directory: %s\n", path);
    return;
  }
  null = strlen(path);
  while((de = readdir(dr))) {
    if(!strcmp(de->d_name, "..") || !strcmp(de->d_name, ".") ||
       (ignore_dotfiles && de->d_name[0] == '.'))
      continue;
    if(de->d_type == DT_LNK || de->d_type == DT_DIR) {
      sprintf(path + null, "/%s", de->d_name);
    }
    if((de->d_type == DT_DIR || (de->d_type == DT_LNK && !stat(path, &sstat) &&
                                 S_ISDIR(sstat.st_mode))) &&
       SCORE_BASE == f(path + 2, expr, len, count) && arr.limit == 1) {
      break;
    }
    if(de->d_type == DT_DIR && iter < max_depth)
      rec_paths(expr, len, count, f, path, iter + 1);
    path[null] = 0;
  }
  closedir(dr);
}

static void find_paths(const char *const *expr, uint len, uint count) {
  uint (*f)(const char *, const char *const *, uint, uint) =
    (unlimited) ? handleinf : handle;
  char path[512] = ".";
  rec_paths(expr, len, count, f, path, 0);
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

  find_paths(argv + off, argc - off, nc);

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
