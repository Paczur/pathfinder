#include "match.h"
#include "res.h"
#include "score.h"
#include "stats.h"
#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/resource.h>

#define CMP_OPTION(buff, short, long) \
  (!strcmp(buff + 1, short) || !strcmp(buff + 1, "-" long))

#define HELP_MSG                                                            \
  "Usage: pf [OPTION]... EXPR...\n"                                         \
  "Find path(s) best matching EXPR using substring matches.\n"              \
  "\n"                                                                      \
  "General options:\n"                                                      \
  "-h, --help            Show help\n"                                       \
  "-v, --verbose         Print errors to stderr\n"                          \
  "-m, --max-matches     Number of matches to print (default 9)\n"          \
  "-r, --reverse         Reverse display order\n"                           \
  "-i, --interactive     Possible options: always, auto, never (default "   \
  "auto)\n"                                                                 \
  "\n"                                                                      \
  "Search Options:\n"                                                       \
  "-I, --ignore-dotfiles   Skip directories and symlinks beginning with "   \
  "\".\"\n"                                                                 \
  "-M, --max-depth         Max depth to search down the tree (default 5)\n" \
  "-t, --types             Types of filesystem object considered when "     \
  "searching, represented by by string of letters(a-all, d-dir, f-file, "   \
  "l-link, D-link to dir, F-link to file) (default dfl)\n"

typedef enum {
  TYPE_FILTER_ALL = 1,
  TYPE_FILTER_DIRS = 2,
  TYPE_FILTER_FILES = 4,
  TYPE_FILTER_LINKS = 8,
  TYPE_FILTER_DIR_LINKS = 16,
  TYPE_FILTER_FILE_LINKS = 32,
} TYPE_FILTER;

typedef enum {
  INTERACTIVE_AUTO,
  INTERACTIVE_ALWAYS,
  INTERACTIVE_NEVER,
} interactive_t;

TYPE_FILTER type_filter =
  TYPE_FILTER_DIRS | TYPE_FILTER_FILES | TYPE_FILTER_LINKS;
bool unlimited;
interactive_t interactive;
bool verbose;
bool reverse;
bool ignore_dotfiles;
uint max_depth = 5;

uint *ranges;
struct stat sstat;
stats_t st;
resl_t list;
resa_t arr = {.size = 0, .capacity = 9};

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

CONST static uint node_count(const char *const *expr, uint len) {
  uint res = len;
  for(uint i = 0; i < len; i++) {
    for(uint j = 0; expr[i][j]; j++) {
      if(expr[i][j] == '/') res++;
    }
  }
  return res;
}

static uint handleinf(const char *restrict str,
                      const char *restrict const *expr, uint len, uint count) {
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

static uint handle(const char *restrict str, const char *restrict const *expr,
                   uint len, uint count) {
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

PURE static bool de_type_matches(const struct dirent *de) {
  return (type_filter & TYPE_FILTER_FILES && de->d_type == DT_REG) ||
         (type_filter & TYPE_FILTER_DIRS && de->d_type == DT_DIR) ||
         (type_filter & (TYPE_FILTER_LINKS | TYPE_FILTER_DIR_LINKS |
                         TYPE_FILTER_FILE_LINKS) &&
          de->d_type == DT_LNK) ||
         type_filter == TYPE_FILTER_ALL;
}

CONST static bool de_useless(const struct dirent *de) {
  return (de->d_name[0] == '.' &&
          (de->d_name[1] == 0 ||
           (de->d_name[1] == '.' && de->d_name[2] == 0))) ||
         (de->d_type != DT_DIR && !de_type_matches(de));
}

static bool link_type_matches(const char *path) {
  return type_filter & TYPE_FILTER_LINKS ||
         (!stat(path, &sstat) &&
          ((type_filter & TYPE_FILTER_DIR_LINKS && S_ISDIR(sstat.st_mode)) ||
           (type_filter & TYPE_FILTER_FILE_LINKS && S_ISREG(sstat.st_mode))));
}

static void dfs_paths(const char *const *expr, uint len, uint count) {
  char path[512] = ".";
  struct dirent *de;
  uint *null_stack;
  uint new_null = 1;
  DIR **dr_stack = malloc(max_depth * sizeof(DIR *));
  uint stack_i = 0;
  dr_stack[0] = opendir(path);
  if(!dr_stack[0]) {
    if(verbose) fprintf(stderr, "Couldn't open directory: %s\n", path);
    free(dr_stack);
    return;
  }
  null_stack = malloc(max_depth * sizeof(uint));

  null_stack[0] = 1;
  while(1) {
    de = readdir(dr_stack[stack_i]);
    if(!de) {
      closedir(dr_stack[stack_i]);
      if(stack_i == 0) break;
      stack_i--;
      continue;
    }
    if(de_useless(de)) continue;

    path[null_stack[stack_i]] = '/';
    new_null = stpcpy(path + null_stack[stack_i] + 1, de->d_name) - path;

    if((de->d_name[0] != '.' || !ignore_dotfiles) && de_type_matches(de) &&
       (!(type_filter & (TYPE_FILTER_DIR_LINKS | TYPE_FILTER_FILE_LINKS)) ||
        link_type_matches(path)) &&
       SCORE_BASE == ((unlimited) ? handleinf(path + 2, expr, len, count)
                                  : handle(path + 2, expr, len, count))) {
      for(uint i = 0; i <= stack_i; i++) closedir(dr_stack[i]);
      break;
    }

    if(de->d_type == DT_DIR && stack_i + 1 < max_depth) {
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
  uint n;
  long long t;
  uint off = 1;
  if(argc < 2) goto error;
  for(; off < (uint)argc; off++) {
    if(argv[off][0] != '-') break;
    if(CMP_OPTION(argv[off], "h", "help")) {
      fputs(HELP_MSG "\n", stderr);
      goto cleanup;
    } else if(CMP_OPTION(argv[off], "m", "max-matches")) {
      if(off + 1 >= (uint)argc || !sscanf(argv[off + 1], "%lld", &t)) {
        goto error;
      }
      if(t < 1) {
        unlimited = true;
      } else {
        arr.capacity = (uint)t;
      }
      off++;
    } else if(CMP_OPTION(argv[off], "v", "verbose")) {
      verbose = true;
    } else if(CMP_OPTION(argv[off], "I", "ignore-dotfiles")) {
      ignore_dotfiles = true;
    } else if(CMP_OPTION(argv[off], "M", "max-depth")) {
      if(off + 1 >= (uint)argc || !sscanf(argv[off + 1], "%lld", &t) || t < 0) {
        goto error;
      }
      max_depth = t;
      off++;
    } else if(!strcmp(argv[off], "-i") || !strcmp(argv[off], "--interactive")) {
      if(off + 1 >= (uint)argc) goto error;
      off++;
      if(!strcmp(argv[off], "always")) {
        interactive = INTERACTIVE_ALWAYS;
      } else if(!strcmp(argv[off], "auto")) {
        interactive = INTERACTIVE_AUTO;
      } else if(!strcmp(argv[off], "never")) {
        interactive = INTERACTIVE_NEVER;
      } else {
        goto error;
      }
    } else if(!strcmp(argv[off], "-r") || !strcmp(argv[off], "--reverse")) {
      reverse = true;
    } else if(CMP_OPTION(argv[off], "t", "types")) {
      if(off + 1 >= (uint)argc) goto error;
      off++;
      type_filter = 0;
      for(uint i = 0; argv[off][i] != 0; i++) {
        switch(argv[off][i]) {
        case 'f':
          type_filter |= TYPE_FILTER_FILES;
          break;
        case 'd':
          type_filter |= TYPE_FILTER_DIRS;
          break;
        case 'l':
          type_filter |= TYPE_FILTER_LINKS;
          break;
        case 'F':
          type_filter |= TYPE_FILTER_FILE_LINKS;
          break;
        case 'D':
          type_filter |= TYPE_FILTER_DIR_LINKS;
          break;
        case 'a':
          type_filter |= TYPE_FILTER_ALL;
          break;
        default:
          goto error;
          break;
        }
      }
    } else if(CMP_OPTION(argv[off], "f", "files-only")) {
      type_filter = TYPE_FILTER_FILES;
    } else if(CMP_OPTION(argv[off], "d", "dirs-only")) {
      type_filter = TYPE_FILTER_DIRS;
    } else if(CMP_OPTION(argv[off], "dl", "dirs-with-links")) {
      type_filter = TYPE_FILTER_DIRS | TYPE_FILTER_LINKS;
    } else if(CMP_OPTION(argv[off], "fl", "files-with-links")) {
      type_filter = TYPE_FILTER_FILES | TYPE_FILTER_LINKS;
    } else {
      goto error;
    }
  }
  if(off >= (uint)argc) {
    goto error;
  }
  nc = node_count(argv + off, argc - off);

  ranges = malloc(nc * 2 * sizeof(uint));
  if(!unlimited) resa_alloc(&arr);
#ifdef NDEBUG
  stats_alloc(&st, nc);
#endif

  dfs_paths(argv + off, argc - off, nc);

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

  if((unlimited && !list.tail) || (!unlimited && !arr.size)) goto cleanup;

  if(interactive == INTERACTIVE_ALWAYS ||
     (interactive == INTERACTIVE_AUTO &&
      ((unlimited && resl_length(&list) > 1) ||
       (!unlimited && resa_length(&arr) > 1)))) {
    if(unlimited) {
      if(reverse) {
        resl_numbered_path_print(&list);
      } else {
        resl_reverse_numbered_path_print(&list);
      }
    } else {
      if(reverse) {
        resa_numbered_path_print(&arr);
      } else {
        resa_reverse_numbered_path_print(&arr);
      }
    }
    fputs("Choice: ", stderr);
    if(!scanf("%u", &n)) goto error;
    if(unlimited) {
      resl_i_path_print(&list, n);
    } else {
      resa_i_path_print(&arr, n);
    }
  } else {
    if(unlimited) {
      if(reverse) {
        resl_reverse_path_print(&list);
      } else {
        resl_path_print(&list);
      }
    } else {
      if(reverse) {
        resa_reverse_path_print(&arr);
      } else {
        resa_path_print(&arr);
      }
    }
  }

cleanup:
  cleanup();
  return 0;

error:
  fputs(HELP_MSG "\n", stderr);
  cleanup();
  return 1;
}
