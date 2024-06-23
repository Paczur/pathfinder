#include "match.h"
#include "res.h"
#include "score.h"
#include "stats.h"
#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

#define HELP_MSG                                                  \
  "Usage: pf [OPTION]... EXPR...\n"                               \
  "Find path(s) best matching EXPR using substring matches.\n"    \
  "\n"                                                            \
  "General options:\n"                                            \
  "-h, --help          Show help\n"                               \
  "-m, --max-matches   Number of matches to print (default 30)\n" \
  "-v, --verbose       Print errors to stderr\n"                  \
  "\n"                                                            \
  "Search Options:\n"                                             \
  "--ignore-dotfiles   Skip directories and symlinks beginning with \".\"\n"

uint *ranges;
struct stat sstat;
stats_t st;
bool unlimited;
bool interactive;
bool verbose;
bool ignore_dotfiles;
resl_t list;
resa_t arr = {.size = 0, .limit = 1};

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
  if(matches(ranges, str, expr, len, count)) {
    new = malloc(sizeof(resn_t));
    new->path = malloc(strlen(str) + 1);
    strcpy(new->path, str);
#ifndef NDEBUG
    stats_alloc(&new->stats, count);
    stats(&new->stats, ranges, count * 2, expr, str);
    new->score = score(&new->stats, count);
#else
    stats(&st, ranges, count * 2, expr, str);
    new->score = score(&st, count);
#endif
    resl_add(&list, new);
    return new->score;
  }
  return 0;
}

static uint handle(const char *str, const char *const *expr, uint len,
                   uint count) {
  resv_t new;
  if(matches(ranges, str, expr, len, count)) {
    new.path = malloc(strlen(str) + 1);
    strcpy(new.path, str);
#ifndef NDEBUG
    stats_alloc(&new.stats, count);
    stats(&new.stats, ranges, count * 2, expr, str);
    new.score = score(&new.stats, count);
#else
    stats(&st, ranges, count * 2, expr, str);
    new.score = score(&st, count);
#endif
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
                      char *path) {
  struct dirent *de;
  DIR *dr = opendir(path);
  size_t null = strlen(path);
  if(!dr) {
    if(verbose) fprintf(stderr, "Couldn't open directory: %s\n", path);
    return;
  }
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
    if(de->d_type == DT_DIR) rec_paths(expr, len, count, f, path);
    path[null] = 0;
  }
  closedir(dr);
}

static void find_paths(const char *const *expr, uint len, uint count) {
  uint (*f)(const char *, const char *const *, uint, uint) =
    (unlimited) ? handleinf : handle;
  char path[512] = ".";
  rec_paths(expr, len, count, f, path);
}

int main(int argc, const char *const argv[]) {
  uint nc;
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
    }
  }
  if(off >= (uint)argc) {
    puts(HELP_MSG);
    goto cleanup;
  }
  nc = node_count(argv + off, argc - off);

  ranges = malloc(nc * 2 * sizeof(uint));
  if(!unlimited) arr.arr = calloc(arr.limit, sizeof(resv_t));
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

  if(unlimited) {
    resl_path_print(&list);
  } else {
    resa_path_print(&arr);
  }

cleanup:
  cleanup();
  return 0;

error:
  puts(HELP_MSG);
  cleanup();
  return 1;
}
